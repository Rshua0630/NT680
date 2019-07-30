#ifndef _LINKAGE_H
#define _LINKAGE_H

#ifndef ALIGN
#define ALIGN .align        5
#endif

#ifndef ENTRY
#define ENTRY(name) \
	.text; \
	.section .text.##name, "ax"; \
	.globl name; \
	ALIGN; \
	name:
#endif

#ifndef WEAK
#define WEAK(name) \
	.weak name; \
	name:
#endif

#ifndef END
#define END(name) \
	.size name, .-name
#endif

#define ENDPROC(name) \
	.type name, %function; \
	END(name)

#endif /* _LINKAGE_H */

