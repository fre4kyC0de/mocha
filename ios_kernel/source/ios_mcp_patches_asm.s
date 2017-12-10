.arm

#patch_os_launch_sig_check:
#	.thumb
#	mov r0, #0
#	mov r0, #0

patch_MCP_authentication_check:
	.thumb
	mov r0, #0
	bx lr

patch_IOSC_VerifyPubkeySign:
patch_cert_verification:
patch_cached_cert_check:
	.arm
	mov r0, #0
	bx lr


.globl mcp_patches_table, mcp_patches_table_end
mcp_patches_table:
#          origin           data                                    size
#    .word 0x0500A818,      patch_os_launch_sig_check,              4
     .word 0x05014CAC,      patch_MCP_authentication_check,         4
     .word 0x05052C44,      patch_IOSC_VerifyPubkeySign,            8
     .word 0x05052A90,      patch_cert_verification,                8
     .word 0x05054D6C,      patch_cached_cert_check,                8
# over an hour, MCP crash prevention
     .word 0x05022474,      0xFFFFFFFF,                             4
mcp_patches_table_end:

