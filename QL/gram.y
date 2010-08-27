%{

#include <stdio.h>

#include "sbpdb.h"
#include "planner.h"
#include "ql.h"
#include "sm.h"

#include "lex.h"

Expression *translateQuery(RelAttrList *al, IDList *rl, Condition *cond) {
	Expression *rels = NULL;
	IDList *p = rl;
	Expression *selexp = NEW(Expression), *ret;
	selexp = NEW(Expression);
	selexp->kind = SelectionExp;
	selexp->u.sele = NEW(struct selection_exp);
	selexp->u.sele->cond = cond;
	while (p) {
		if (rels) {
			Expression *tmp = NEW(Expression);
			Expression *left, *right;
			tmp->kind = ProductExp;
			tmp->u.prode = NEW(struct product_exp);
			left = rels;
			right = NEW(Expression);
			right->kind = Relation;
			right->u.rel = NEW(struct relation);
			right->u.rel->id = p->id;
			tmp->u.prode->left = left;
			tmp->u.prode->right = right;
			rels = tmp;
		} else {
			rels = NEW(Expression);
			rels->kind = Relation;
			rels->u.rel = NEW(struct relation);
			rels->u.rel->id = p->id;
		}
		p = p->next;
	}
	selexp->u.sele->exp = rels;
	if (al) {
		ret = NEW(Expression);
		ret->kind = ProjectionExp;
		ret->u.proje = NEW(struct projection_exp);
		ret->u.proje->al = al;
		ret->u.proje->exp = selexp;
		return ret;
	} else {
		return selexp;
	}
}

void yyerror(const char *str) {
    fprintf(stderr, "Error: %s\n", str);
}

PF_Manager *pfManager;
SM_Manager *smManager;
QL_Manager *qlManager;

%}

%token SEMICOLON CREATE DROP TABLE VIEW SELECT INSERT DELETE UPDATE
       LBRACE RBRACE COMMA INT_T STRING_T FLOAT_T HELP EXIT DOT
       INTO VALUES STAR FROM WHERE AND OR NOT AS PRIMARY KEY
       EQ LT GT LE GE NE IN DATABASE USE SET FOREIGN REFERENCES
       CHECK

%union {
    AttrInfo *attrInfo;
    AttrType attrType;
    Value *value;
	ValueList *valueList;
    RelAttr *relAttr;
	RelAttrList *relAttrList;
	RelAttrValue *relAttrValue;
	RelAttrValueList *relAttrValueList;
	AssignmentList *assignmentList;
	IDList *idList;
    Condition *condition;
	CheckCondition *checkCondition;
	Expression *expression;
    int compOp;
    int number;
    char *string;
}

%token <number> NUMBER
%token <string> ID
%token <string> STRING_V

%nonassoc QUERY
%left OR
%left AND
%right NOT

%type <attrInfo> attr_def attr_def_list;
%type <attrType> type;
%type <value> value;
%type <valueList> value_list;
%type <relAttr> attr fk;
%type <relAttrList> attr_list select_attr;
%type <relAttrValue> av;
%type <relAttrValueList> avlist;
%type <assignmentList> assignment_list;
%type <condition> condition;
%type <idList> rel_list;
%type <compOp> compOp;
%type <expression> query;
%type <number> pk;
%type <checkCondition> ck;

%start commands;

%%

commands:
	command {
		printf("> ");
		fflush(stdout);
	}
	| commands command {
		printf("> ");
		fflush(stdout);
	}
	;

command:
	empty
	| use_database
	| create_database
    | create_table
    | create_view
	| drop_database
    | drop_table
    | drop_view
    | select
    | insert
    | delete
	| update
    | help
    | exit
    ;

empty:
	SEMICOLON
	;

exit:
	EXIT SEMICOLON{
        SM_Exit(smManager);
        return 0;
    }
	;

help:
	HELP SEMICOLON{
        SM_Help(smManager);
    }
	;

use_database:
	USE ID SEMICOLON {
		int ret;
		if ((ret = SM_UseDatabase(smManager, $2)) != NORMAL) {
			fprintf(stderr, "Error: database %s doesn't exist\n", $2);
		}
	}
	;

create_database:
	CREATE DATABASE ID SEMICOLON {
		int ret;
		if ((ret = SM_CreateDatabase(smManager, $3)) != NORMAL) {
			fprintf(stderr, "Error: database %s already exists\n", $3);
		}
	}
	;

drop_database:
	DROP DATABASE ID SEMICOLON {
		int ret;
		if ((ret = SM_DropDatabase(smManager, $3)) != NORMAL) {
			if (ret == SM_NODIR)
				fprintf(stderr, "Error: database %s doesn't exist\n", $3);
		}
	}
	;

create_table:
	CREATE TABLE ID LBRACE attr_def_list RBRACE SEMICOLON {
		int ret;
		if ((ret = SM_CreateTable(smManager, $3, $5)) != NORMAL) {
			if (ret == SM_NODBSELECTED)
				fprintf(stderr, "Error: no database selected\n");
			else if (ret == SM_DUPLICATEATTR)
				fprintf(stderr, "Error: duplicate column name\n");
			else if (ret == PF_EXIST)
				fprintf(stderr, "Error: table %s already exists\n", $3);
			else if (ret == SM_VIEWEXIST)
				fprintf(stderr, "Error: view %s already exists\n", $3);
		}
    }
	;

drop_table:
	DROP TABLE ID SEMICOLON {
		int ret;
		if ((ret = SM_DropTable(smManager, $3)) != NORMAL) {
			if (ret == SM_NODBSELECTED)
				fprintf(stderr, "Error: no database selected\n");
			else if (ret == PF_NOTEXIST)
				fprintf(stderr, "Error: table %s doesn't exist\n", $3);
		}
    }
	;

create_view:
	CREATE VIEW ID AS query SEMICOLON {
		int ret;
        if ((ret = SM_CreateView(smManager, $3, $5)) != NORMAL) {
        	if (ret == SM_NODBSELECTED)
				fprintf(stderr, "Error: no database selected\n");
			else if (ret == SM_VIEWEXIST)
				fprintf(stderr, "Error: view %s already exists\n", $3);
			else if (ret == SM_TABLEEXIST)
				fprintf(stderr, "Error: table %s already exists\n", $3);
		}
    }
	;

drop_view:
	DROP VIEW ID SEMICOLON {
		int ret;
        if ((ret = SM_DropView(smManager, $3)) != NORMAL) {
        	if (ret == SM_NODBSELECTED)
				fprintf(stderr, "Error: no database selected\n");
			else if (ret == SM_VIEWNOTEXIST)
				fprintf(stderr, "Error: view %s doesn't exist\n", $3);
		}
    }
	;

attr_def_list:
    attr_def {
		$$ = $1;
	}
    | attr_def_list COMMA attr_def {
		AttrInfo *p = GET_LAST(AttrInfo, $1, next);
		p->next = $3;
		$$ = $1;
    };

attr_def:
    ID type pk fk ck {
		$$ = NEW(AttrInfo);
        $$->name = $1;
        $$->type = $2;
        switch ($2) {
        case INT: $$->size = INT_SIZE; break;
        case STRING: $$->size = MAXSTRINGLEN; break;
        case FLOAT: $$->size = FLOAT_SIZE; break;
        }
		$$->isPrimaryKey = $3;
		$$->foriegnKey = $4;
		$$->check = $5;
    };

pk:
	{ $$ = 0; }
	| PRIMARY KEY {
		$$ = 1;
	}
	;

fk:
	{ $$ = NULL; }
	| FOREIGN KEY REFERENCES ID LBRACE ID RBRACE {
		$$ = NEW(RelAttr);
		$$->relName = $4;
		$$->attrName = $6;
	}
	;

ck:
	{ $$ = NULL; }
	| CHECK LBRACE ID compOp value RBRACE {
		$$ = NEW(CheckCondition);
		$$->attr = $3;
		$$->op = $4;
		$$->value = $5;
	}
	;

type:
    INT_T {
        $$ = INT;
    }
    | STRING_T {
        $$ = STRING;
    }
    | FLOAT_T {
        $$ = FLOAT;
    };

insert:
    INSERT INTO ID VALUES LBRACE value_list RBRACE SEMICOLON {
		int ret;
		if ((ret = QL_Insert(qlManager, $3, $6)) != NORMAL) {
			if (ret == QL_DUPLICATE)
				fprintf(stderr, "Error: duplicate primary key\n");
			else if (ret == QL_CHECKFAIL)
				fprintf(stderr, "Error: check failed\n");
			else if (ret == QL_FOREIGNNOFOUND)
				fprintf(stderr, "Error: foreign key no found\n");
		}
    }
	;

value_list:
    value {
		$$ = NEW(ValueList);
		$$->v = $1;
		$$->next = NULL;
	}
    | value_list COMMA value {
        ValueList *p = GET_LAST(ValueList, $1, next);
		p->next = NEW(ValueList);
		p->next->v = $3;
		p->next->next = NULL;
		$$ = $1;
    }
	;

value:
    NUMBER {
        $$ = NEW(Value);
        $$->type = INT;
        int *p = malloc(sizeof(int));
        *p = $1;
        $$->data = p;
    }
    | STRING_V {
        $$ = NEW(Value);
        $$->type = STRING;
		$$->data = malloc(STRING_SIZE);
		strncpy($$->data, $1, MAXSTRINGLEN);
		free($1);
		((char *)($$->data))[MAXSTRINGLEN] = 0;
    }
    | NUMBER DOT NUMBER {
		//TODO
    }
	;

delete:
    DELETE FROM ID WHERE condition SEMICOLON {
		IDList *il = NEW(IDList);
		il->id = $3;
		Expression *exp = translateQuery(NULL, il, $5);
		int ret;
        if ((ret = QL_Delete(qlManager, $3, exp)) != NORMAL) {
			if (ret == QL_ONLYFK)
				fprintf(stderr, "can't delete foreign key\n");
		}
    }
	| DELETE FROM ID SEMICOLON {
		IDList *il = NEW(IDList);
		il->id = $3;
		Expression *exp = translateQuery(NULL, il, NULL);
		int ret;
        if ((ret = QL_Delete(qlManager, $3, exp)) != NORMAL) {
			if (ret == QL_ONLYFK)
				fprintf(stderr, "can't delete foreign key\n");
		}
    }
	| DELETE STAR FROM ID SEMICOLON {
		IDList *il = NEW(IDList);
		il->id = $4;
		Expression *exp = translateQuery(NULL, il, NULL);
		int ret;
        if ((ret = QL_Delete(qlManager, $4, exp)) != NORMAL) {
			if (ret == QL_ONLYFK)
				fprintf(stderr, "can't delete foreign key\n");
		}
    }
	;

update:
	UPDATE ID SET assignment_list WHERE condition SEMICOLON {
		int ret;
		IDList *il = NEW(IDList);
		il->id = $2;
		Expression *exp = translateQuery(NULL, il, $6);
		if ((ret = QL_Update(qlManager, $2, $4, exp)) != NORMAL) {
			if (ret == QL_DUPLICATE)
				fprintf(stderr, "Error: duplicate primary key\n");
			else if (ret == QL_CHECKFAIL)
				fprintf(stderr, "Error: check failed\n");
			else if (ret == QL_ONLYFK)
				fprintf(stderr, "can't update foreign key\n");
		}
	}
	;

assignment_list:
	attr EQ av {
		$$ = NEW(AssignmentList);
		$$->left = $1;
		$$->right = $3;
		$$->next = NULL;
	}
	| assignment_list COMMA attr EQ av {
		AssignmentList *p = GET_LAST(AssignmentList, $1, next);
		p->next = NEW(AssignmentList);
		p->next->left = $3;
		p->next->right = $5;
		p->next->next = NULL;
	}
	;

query:
	SELECT select_attr FROM rel_list WHERE condition %prec QUERY {
		$$ = translateQuery($2, $4, $6);
	}
	| SELECT select_attr FROM rel_list {
		$$ = translateQuery($2, $4, NULL);
	}
	;

select:
    query SEMICOLON {
		int ret;
		if ((ret = QL_Select(qlManager, $1)) != NORMAL) {
		}
    }
	;

select_attr:
    STAR {
		$$ = NULL;
	}
    | attr_list {
        $$ = $1;
    }
	;

condition:
    av compOp av {
        $$ = NEW(Condition);
		$$->kind = CompOpCond;
		$$->u.coc = NEW(CompOpCondition);
        $$->u.coc->left = $1;
        $$->u.coc->op = $2;
        $$->u.coc->right = $3;
    }
	| avlist IN LBRACE query RBRACE {
        $$ = NEW(Condition);
		$$->kind = InCond;
		$$->u.ic = NEW(InCondition);
		$$->u.ic->avl = $1;
		$$->u.ic->rel = $4;
	}
	| condition AND condition {
        $$ = NEW(Condition);
		$$->kind = AndCond;
		$$->u.ac = NEW(AndCondition);
		$$->u.ac->left = $1;
		$$->u.ac->right = $3;
	}
	| condition OR condition {
        $$ = NEW(Condition);
		$$->kind = OrCond;
		$$->u.oc = NEW(OrCondition);
		$$->u.oc->left = $1;
		$$->u.oc->right = $3;
	}
	| NOT condition {
        $$ = NEW(Condition);
		$$->kind = NotCond;
		$$->u.nc = NEW(NotCondition);
		$$->u.nc->cond = $2;
	}
	| LBRACE condition RBRACE {
		$$ = $2;
	}
	;

compOp:
    EQ { $$ = EQ_OP; }
    | LT { $$ = LT_OP; }
    | GT { $$ = GT_OP; }
    | LE { $$ = LE_OP; }
    | GE { $$ = GE_OP; }
    | NE { $$ = NE_OP; }
    ;

av:
    attr {
		$$ = NEW(RelAttrValue);
		$$->isValue = 0;
		$$->u.a = $1;
    }
    | value {
		$$ = NEW(RelAttrValue);
		$$->isValue = 1;
		$$->u.v = $1;
    };

avlist:
	av {
		$$ = NEW(RelAttrValueList);
		$$->av = $1;
		$$->next = NULL;
	}
	| avlist COMMA av {
        RelAttrValueList *p = GET_LAST(RelAttrValueList, $1, next);
		p->next = NEW(RelAttrList);
		p->next->av = $3;
		p->next->next = NULL;
		$$ = $1;
	}
	;

attr:
    ID {
		$$ = NEW(RelAttr);
        $$->relName = NULL;
        $$->attrName = $1;
    }
    | ID DOT ID {
		$$ = NEW(RelAttr);
        $$->relName = $1;
        $$->attrName = $3;
    };

attr_list:
    attr {
		$$ = NEW(RelAttrList);
		$$->a = $1;
		$$->next = NULL;
	}
    | attr_list COMMA attr {
        RelAttrList *p = GET_LAST(RelAttrList, $1, next);
		p->next = NEW(RelAttrList);
		p->next->a = $3;
		p->next->next = NULL;
		$$ = $1;
    };

rel_list:
    ID {
		$$ = NEW(IDList);
		$$->id = $1;
		$$->next = NULL;
	}
    | rel_list COMMA ID {
		IDList *p = GET_LAST(IDList, $1, next);
		p->next = NEW(IDList);
		p->next->id = $3;
		p->next->next = NULL;
		$$ = $1;
    };

%%

int sbp_parse(PF_Manager *pfm, SM_Manager *smm, QL_Manager *qlm) {
    pfManager = pfm;
    smManager = smm;
    qlManager = qlm;
    return yyparse();
}
