

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>

/* For the UUID (found in the TA's h-file(s)) */
#include <hello_world_ta.h>

int main(void)
{
	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op;
	TEEC_UUID uuid = TA_HELLO_WORLD_UUID;
	uint32_t err_origin;

	// FILE *fp;
	// int buff[255];
    // fp = fopen("test.txt", "w+");
    // if(fp == NULL)
    // {
    //     printf("Unable to open file.\n");
    // }
	// fscanf(fp,"%d", buff);
	
	/* Initialize a context connecting us to the TEE */
	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	/*
	 * Open a session to the "hello world" TA, the TA will print "hello
	 * world!" in the log when the session is created.
	 */
	res = TEEC_OpenSession(&ctx, &sess, &uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
			res, err_origin);

	/*
	 * Execute a function in the TA by invoking it, in this case
	 * we're incrementing a number.
	 *
	 * The value of command ID part and how the parameters are
	 * interpreted is part of the interface provided by the TA.
	 */

	/* Clear the TEEC_Operation struct */
	memset(&op, 0, sizeof(op));

	/*
	 * Prepare the argument. Pass a value in the first parameter,
	 * the remaining three parameters are unused.
	 */
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);
	// op.params[0].value.a = buff;
	op.params[0].value.a = 24;
	/*
	 * TA_HELLO_WORLD_CMD_INC_VALUE is the actual function in the TA to be
	 * called.
	 */
	char c;
	do{
		c = getchar();
		if(c == 10){
			printf("Current Number is: %d\n", op.params[0].value.a);
			res = TEEC_InvokeCommand(&sess, TA_HELLO_WORLD_CMD_INC_VALUE, &op,
				 &err_origin);
			if (res != TEEC_SUCCESS)
				errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
					res, err_origin);
			printf("Next Number is:  %d\n", op.params[0].value.a);
			// res = TEEC_InvokeCommand(&sess, TA_HELLO_WORLD_CMD_DEC_VALUE, &op,
			// 	 &err_origin);
			// if (res != TEEC_SUCCESS)
			// 	errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			// 		res, err_origin);
			// printf("Next Number is:  %d\n", op.params[0].value.a);
		}
	}while(c!=113);
	
	// fp = fopen("test.txt","w+");
	// int cc = op.params[0].value.a;
	// fprintf(fp,"%d",cc);
	/*
	 * We're done with the TA, close the session and
	 * destroy the context.
	 *
	 * The TA will print "Goodbye!" in the log when the
	 * session is closed.
	 */

	TEEC_CloseSession(&sess);

	TEEC_FinalizeContext(&ctx);

	return 0;
}
