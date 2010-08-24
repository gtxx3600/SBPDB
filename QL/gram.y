%{

#include <stdio.h>

#include "sbpdb.h"
#include "sm.h"
#include "ql.h"

#include "lex.c"

#define NEW(type) \
	calloc(sizeof(type), 1)

#define GET_LAST(type, first, next) ({ \
	(type *) __p = first; \
	while (__p->next) __p = __p->next; \
	__p; })

Expression *translateQuery(RelAttrList *avl, RelList *rl, Condition *cond) {
	return NULL;
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
       INTO VALUES STAR FROM WHERE AND OR NOT AS
       EQ LT GT LE GE NE IN

%union {
    AttrInfo *attrInfo;
    AttrType *attrType;
    Value *value;
	ValueList *valueList;
    RelAttr *relAttr;
	RelAttrList *relAttrList;
	RelAttrValue *relAttrValue;
	RelAttrValueList *relAttrValueList;
	IDList *idList;
    Condition *condition;
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
%type <relAttr> attr;
%type <relAttrList> attr_list select_attr;
%type <relAttrValue> av;
%type <relAttrValueList> avlist;
%type <condition> condition;
%type <idList> rel_list;
%type <compOp> compOp;
%type <expression> query;

%%

commands:
    /* empty */
    | commands command
    ;

command:
    create_table
    | create_view
    | drop_table
    | drop_view
    | select
    | insert
    | delete
    | help
    | exit
    ;

exit:
	EXIT SEMICOLON {
        smManager->Exit();
    }
	;

help:
	HELP SEMICOLON {
        smManager->Help();
    }
	;

create_table:
	CREATE TABLE ID LBRACE attr_def_list RBRACE SEMICOLON {
		smManager->CreateTable(smManager, $3, $5);
    }
	;

drop_table:
	DROP TABLE ID SEMICOLON {
        smManager->DropTable(smManager, $3);
    }
	;

create_view:
	CREATE VIEW ID AS query SEMICOLON {
        smManager->CreateView(smManager, $3, $5); // TODO
    }
	;

drop_view:
	DROP VIEW ID SEMICOLON {
        smManager->DropView(smManager, $3); // TODO
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
    ID type {
		$$ = NEW(AttrInfo);
        $$->name = $1;
        $$->type = $2;
        switch ($2) {
        case INT: $$->size = INT_SIZE; break;
        case STRING: $$->size = MAXSTRINGLEN; break;
        case FLOAT: $$->size = FLOAT_SIZE; break;
        }
    };

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
		qlManager->Insert(qlManager, $3, $6);
    }
	;

value_list:
    value {
		$$ = NEW(ValueList);
		$$->v = $1;
		$$->next = NULL;
	}
    | value_list COMMA value {
        Value *p = GET_LAST(ValueList, $1, next);
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
        $$->data = $1;
    }
    | NUMBER DOT NUMBER {
		//TODO
    }
	;

delete:
    DELETE FROM ID WHERE condition SEMICOLON {
        qlManager->Delete(qlManager, $3, NULL);
    }
	| DELETE FROM ID SEMICOLON {
        qlManager->Delete(qlManager, $3, NULL);
    }
	| DELETE STAR FROM ID SEMICOLON {
        qlManager->Delete(qlManager, $4, NULL);
    }
	;

query:
	SELECT select_attr FROM rel_list WHERE condition %prec QUERY{
		$$ = translateQuery($2, $4, $6);
	}
	;

select:
    query SEMICOLON {
		qlManager->Select(qlManager, $1);
    };

select_attr:
    STAR {
		$$ = NULL;
	}
    | attr_list {
        $$ = $1;
    };

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

int parse(PF_Manager &pfm, SM_Manager &smm, QL_Manager &qlm) {
    pfManager = &pfm;
    smManager = &smm;
    qlManager = &qlm;
    return yyparse();
}
