/* $Id: mach3.c, v2.0 23-Feb-89 AJM */

0,		COND,		0x00000000,	".eq",
0,		COND,		0x10000000,	".ne",
0,		COND,		0x20000000,	".cs",
0,		COND,		0x20000000,	".hs",
0,		COND,		0x30000000,	".cc",
0,		COND,		0x30000000,	".lo",
0,		COND,		0x40000000,	".mi",
0,		COND,		0x50000000,	".pl",
0,		COND,		0x60000000,	".vs",
0,		COND,		0x70000000,	".vc",
0,		COND,		0x80000000,	".hi",
0,		COND,		0x90000000,	".ls",
0,		COND,		0xA0000000,	".ge",
0,		COND,		0xB0000000,	".lt",
0,		COND,		0xC0000000,	".gt",
0,		COND,		0xD0000000,	".le",
0,		COND,		0xE0000000,	".al",
0,		COND,		0xF0000000,	".nv",

0,		LINK,		0x01000000,	".l",
0,		BRANCH,		0xEB000000,	"bl",

0,		BRANCH,		0x0A000000,	"beq",
0,		BRANCH,		0x1A000000,	"bne",
0,		BRANCH,		0x2A000000,	"bcs",
0,		BRANCH,		0x2A000000,	"bhs",
0,		BRANCH,		0x3A000000,	"bcc",
0,		BRANCH,		0x3A000000,	"blo",
0,		BRANCH,		0x4A000000,	"bmi",
0,		BRANCH,		0x5A000000,	"bpl",
0,		BRANCH,		0x6A000000,	"bvs",
0,		BRANCH,		0x7A000000,	"bvc",
0,		BRANCH,		0x8A000000,	"bhi",
0,		BRANCH,		0x9A000000,	"bls",
0,		BRANCH,		0xAA000000,	"bge",
0,		BRANCH,		0xBA000000,	"blt",
0,		BRANCH,		0xCA000000,	"bgt",
0,		BRANCH,		0xDA000000,	"ble",
0,		BRANCH,		0xEA000000,	"b",
0,		BRANCH,		0xFA000000,	"bnv",

0,		DATA1,		ADC,	"adc",
0,		DATA1,		ADD,	"add",
0,		DATA1,		AND,	"and",
0,		DATA1,		BIC,	"bic",
0,		DATA1,		EOR,	"eor",
0,		DATA1,		ORR,	"orr",
0,		DATA1,		RSB,	"rsb",
0,		DATA1,		RSC,	"rsc",
0,		DATA1,		SBC,	"sbc",
0,		DATA1,		SUB,	"sub",
0,		DATA2,		MOV,	"mov",
0,		DATA2,		MVN,	"mvn",
0,		DATA3,		CMN,	"cmn",
0,		DATA3,		CMP,	"cmp",
0,		DATA3,		TEQ,	"teq",
0,		DATA3,		TST,	"tst",

0,		SET,		0x00100000,	".s",

0,		PEE,		0x0010F000,	".p",

0,		REG,		0,		"r0",
0,		REG,		1,		"r1",
0,		REG,		2,		"r2",
0,		REG,		3,		"r3",
0,		REG,		4,		"r4",
0,		REG,		5,		"r5",
0,		REG,		6,		"r6",
0,		REG,		7,		"r7",
0,		REG,		8,		"r8",
0,		REG,		9,		"r9",
0,		REG,		10,		"r10",
0,		REG,		11,		"r11",
0,		REG,		12,		"r12",
0,		REG,		13,		"r13",
0,		REG,		14,		"r14",
0,		REG,		15,		"r15",

0,		REG,		11,		"fp",
0,		REG,		12,		"ip",
0,		REG,		13,		"sp",
0,		REG,		14,		"lr",
0,		REG,		15,		"pc",

0,		SHIFT,		0x00000000,	"lsl",
0,		SHIFT,		0x00000000,	"asl",
0,		SHIFT,		0x00000020,	"lsr",
0,		SHIFT,		0x00000040,	"asr",
0,		SHIFT,		0x00000060,	"ror",

0,		RRX,		0x00000060,	"rrx",

0,		SDT,		0x04100000,	"ldr",
0,		SDT,		0x04500000,	"ldrb",
0,		SDT,		0x04000000,	"str",
0,		SDT,		0x04400000,	"strb",

0,		SDT2,		0x001000b0,	"ldrh",
0,		SDT2,		0x000000b0,	"strh",
0,		SDT2,		0x001000d0,	"ldrsb",
0,		SDT2,		0x000000d0,	"strsb",

0,		TRANS,		0x00200000,	".t",

0,		BDT,		0x09100000,	"ldmdb",
0,		BDT,		0x08100000,	"ldmda",
0,		BDT,		0x09900000,	"ldmib",
0,		BDT,		0x08900000,	"ldmia",
0,		BDT,		0x08900000,	"ldmfd",
0,		BDT,		0x08100000,	"ldmfa",
0,		BDT,		0x09900000,	"ldmed",
0,		BDT,		0x09100000,	"ldmea",
0,		BDT,		0x09000000,	"stmdb",
0,		BDT,		0x08000000,	"stmda",
0,		BDT,		0x09800000,	"stmib",
0,		BDT,		0x08800000,	"stmia",
0,		BDT,		0x09000000,	"stmfd",
0,		BDT,		0x09800000,	"stmfa",
0,		BDT,		0x08000000,	"stmed",
0,		BDT,		0x08800000,	"stmea",

0,		SWI,		0x0F000000,	"swi",

0,		ADR,		0x00000000,	"adr",

0,		MUL,		0x00000090,	"mul",
0,		MLA,		0x00200090,	"mla",


