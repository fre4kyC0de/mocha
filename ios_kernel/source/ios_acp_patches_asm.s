.arm

patch_bootMovie_check:
patch_bootLogoTex_check:
patch_region_launch_check:
	.arm
	mov r0, #0
	bx lr

.globl acp_patches_table, acp_patches_table_end
acp_patches_table:
	.word 0xE0030D68,		patch_bootMovie_check,					4
	.word 0xE0030D34,		patch_bootLogoTex_check,				4
	.word 0xE0030498,		patch_region_launch_check,				4
acp_patches_table_end:
