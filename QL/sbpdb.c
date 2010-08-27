#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ql.h"

int main() {
	PF_Manager pfm;
	RM_Manager rmm;
	IX_Manager ixm;
	SM_Manager smm;
	QL_Manager qlm;
	initPF_Manager(&pfm);
	initRM_Manager(&rmm, &pfm);
	initIX_Manager(&ixm, &pfm);
	initSM_Manager(&smm, &ixm, &rmm);
	initQL_Manager(&qlm, &smm, &ixm, &rmm);

	if (access(BASEDIR, 0))
		system("mkdir "BASEDIR);
	chdir(BASEDIR);
//	system("ls");
	while (!smm.isExit) {
		printf("> ");
		fflush(stdout);
		sbp_parse(&pfm, &smm, &qlm);
	}
	SM_CloseDb(&smm);
	printf("Bye\n");
	return 0;
}
