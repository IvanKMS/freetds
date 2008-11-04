#include "common.h"

static char software_version[] = "$Id: norowset.c,v 1.8 2008-11-04 14:46:17 freddy77 Exp $";
static void *no_unused_var_warn[] = { software_version, no_unused_var_warn };

/* Test that a select following a store procedure execution return results */

int
main(int argc, char *argv[])
{
	char output[256];
	SQLLEN dataSize;

	Connect();

	CommandWithResult(Statement, "drop proc sp_norowset_test");

	Command("create proc sp_norowset_test as begin declare @i int end");

	Command("exec sp_norowset_test");

	/* note, mssql 2005 seems to not return row for tempdb, use always master */
	Command("select name from master..sysobjects where name = 'sysobjects'");
	CHKFetch("S");

	CHKGetData(1, SQL_C_CHAR, output, sizeof(output), &dataSize, "S");

	if (strcmp(output, "sysobjects") != 0) {
		printf("Unexpected result\n");
		exit(1);
	}

	CHKFetch("No");

	CHKMoreResults("No");

	Command("drop proc sp_norowset_test");

	Disconnect();

	printf("Done.\n");
	return 0;
}
