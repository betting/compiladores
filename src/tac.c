TAC* CodeGenerate(ASTREE* node,TAC* code, int iloc_code)
{

	switch(iloc_code)
	{
		case ILOC_NOP:
		case ILOC_ADD:
		case ILOC_SUB:
		case ILOC_MULT:
		case ILOC_DIV:
		case ILOC_ADDI:
		case ILOC_SUBI:
		case ILOC_RSUBI:
		case ILOC_MULTI:
		case ILOC_DIVI:
		case ILOC_RDIVI:
		case ILOC_LSHIFT:
		case ILOC_LSHIFTI:
		case ILOC_RSHIFT:
		case ILOC_RSHIFTI:
		case ILOC_AND:
		case ILOC_ANDI:
		case ILOC_OR:
		case ILOC_ORI:
		case ILOC_XOR:
		case ILOC_XORI:
		case ILOC_LOADI:
		case ILOC_LOAD:
		case ILOC_LOADAI:
		case ILOC_LOADAO:
		case ILOC_CLOAD:
		case ILOC_CLOADAI:
		case ILOC_CLOADAO:
		case ILOC_STORE:
		case ILOC_STOREAI:
		case ILOC_STOREAO:
		case ILOC_CSTORE:
		case ILOC_CSTOREAI:
		case ILOC_CSTOREAO:
		case ILOC_I2I:
		case ILOC_C2C:
		case ILOC_C2I:
		case ILOC_I2C:
		case ILOC_JUMPI:
		case ILOC_JUMP:
		case ILOC_CBR:
		case ILOC_CMP_LT:
		case ILOC_CMP_LE:
		case ILOC_CMP_EQ:
		case ILOC_CMP_GE:
		case ILOC_CMP_GT:
		case ILOC_CMP_NE:
					break;
	}


}
