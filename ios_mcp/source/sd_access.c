void patch_SD_access_check(void)
{
	__asm__ volatile(
		".thumb\n"
		//clobbered instructions
		"add r0, r7, r2\n"
		//app.permissions.r2.mask seems to be 0xFFFFFFFFFFFFFFFF for every application
		"ldr r1, =0x32\n"
		"sub r3, r3, #7\n"
		"strb r1, [r3]\n"
		//this instruction was also clobbered but we use r1 so we do it after our patch stuff
		"movs r1, #0\n"
		"bx lr");
}
