
#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <strsafe.h>

/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>

/* For the UUID (found in the TA's h-file(s)) */
#include <hello_world_ta.h>

size_t strlcpy(char *restrict dst, const char *restrict src, size_t dstsize)
{
    int offset;

    /* duplicate the string up to dstsize */
    offset = 0;
        /* guard against silly dstsize values */
    if( dstsize > 0 )
    {
        while( *(src+offset) != '\0' )
        {
            /* bail if dstsize is met */
            if( offset==dstsize )
            {
                offset--;
                break;
            }

            /* duplicate the string */
            *(dst+offset) = *(src+offset);
            offset++;
        }
    }
    /* always remember to cap a created string! */
    *(dst+offset) = '\0';
    
    /* return the string length of src */
    while( *(src+offset) != '\0' )
        offset++;

    return(offset);
}

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
	
	cchar realPassword[20];
    char pass[]= "ddddddddddddddd";
    strlcpy(realPassword,pass,sizeof(realPassword));
	if (0 == strncmp(argv[1], realPassword, 20)){
		printf("SUCCESS!\n");
	}else{
		printf("FAILURE!\n");
	}
	raise(SIGINT);

	TEEC_CloseSession(&sess);

	TEEC_FinalizeContext(&ctx);

	return 0;
}
