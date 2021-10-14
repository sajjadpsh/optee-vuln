#include <stdio.h>
#include <signal.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <strsafe.h>
/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>

/* For the UUID (found in the TA's h-file(s)) */
#include <hello_world_ta.h>
void norExecutableInMain()
{
    printf("**Server SSH Password**\n");
    printf("Server Password : qwerty123\n");
}

void doit(char* buffer){

    char buf[20];
    strncpy(buf, buffer,sizeof(buf));
    printf("your Identifier is: '%s'\n", buf);}

int main(int argc, char *argv[])
{
    TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op;
	TEEC_UUID uuid = TA_HELLO_WORLD_UUID;
	uint32_t err_origin;

	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	res = TEEC_OpenSession(&ctx, &sess, &uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
			res, err_origin);

	memset(&op, 0, sizeof(op));

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);
    doit(argv[1]);

    raise(SIGINT); 

	TEEC_CloseSession(&sess);

	TEEC_FinalizeContext(&ctx);
    return 0;
}