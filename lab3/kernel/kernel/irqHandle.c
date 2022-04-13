#include "common.h"
#include "x86.h"
#include "device.h"

#define va_to_pa(va) (va + (current + 1) * 0x100000)
#define pa_to_va(pa) (pa - (current + 1) * 0x100000)

extern TSS tss;
extern int displayRow;
extern int displayCol;

extern ProcessTable pcb[MAX_PCB_NUM];
extern int current; // current process


void GProtectFaultHandle(struct StackFrame *sf);

void timerHandle(struct StackFrame *sf);

void syscallHandle(struct StackFrame *sf);

void syscallWrite(struct StackFrame *sf);
void syscallPrint(struct StackFrame *sf);
void syscallFork(struct StackFrame *sf);
void syscallExec(struct StackFrame *sf);
void syscallSleep(struct StackFrame *sf);
void syscallExit(struct StackFrame *sf);


void irqHandle(struct StackFrame *sf) { // pointer sf = esp
	/* Reassign segment register */
	asm volatile("movw %%ax, %%ds"::"a"(KSEL(SEG_KDATA)));
	
	/* Save esp to stackTop */
	//为了中断嵌套
	pcb[current].stackTop=(uint32_t)sf;

	switch(sf->irq) {
		case -1:
			break;
		case 0xd:
			GProtectFaultHandle(sf);
			break;
		case 0x20:
			timerHandle(sf);
			break;
		case 0x80:
			syscallHandle(sf);
			break;
		default:assert(0);
	}
}

void GProtectFaultHandle(struct StackFrame *sf) {
	assert(0);
	return;
}

void SwitchProcess(uint32_t current) {
	pcb[current].state = STATE_RUNNING;
	pcb[current].timeCount = 0;
	uint32_t tmpStackTop=pcb[current].stackTop;
	tss.esp0=(uint32_t)&(pcb[current].stackTop);
	asm volatile("movl %0,%%esp"::"m"(tmpStackTop));
	asm volatile("popl %gs");
	asm volatile("popl %fs");
	asm volatile("popl %es");
	asm volatile("popl %ds");
	asm volatile("popal");
	asm volatile("addl $8,%esp");
	asm volatile("iret");
}

void timerHandle(struct StackFrame *sf){
	//TODO 完成进程调度，建议使用时间片轮转，按顺序调度
	// int oldpid = current;
	for (int i = 1; i < MAX_PCB_NUM; i++) {
		if (pcb[i].state == STATE_BLOCKED) {
			if ((--pcb[i].sleepTime) == 0)
				pcb[i].state = STATE_RUNNABLE;
		}
	}

	if ((++pcb[current].timeCount) >= MAX_TIME_COUNT || current == 0 || \
	pcb[current].state == STATE_BLOCKED || pcb[current].state == STATE_DEAD) {
		for (int i = 1; i < MAX_PCB_NUM; i++) {
			if (pcb[i].state == STATE_RUNNABLE && i != current) {
				if (pcb[current].state == STATE_RUNNING)
					pcb[current].state = STATE_RUNNABLE;
				current = i;
				SwitchProcess(current);
				return;
			}
		}

		if (current != 0) {
			pcb[current].state = STATE_RUNNABLE;
			current = 0;
			SwitchProcess(current);
		}
	}
}


void syscallHandle(struct StackFrame *sf) {
	switch(sf->eax) { // syscall number
		case 0:
			syscallWrite(sf);
			break; // for SYS_WRITE
		/* Add Fork,Sleep... */
		case 1:
			syscallFork(sf);
			break;
		case 2:
			syscallExec(sf);
			break;
		case 3:
			syscallSleep(sf);
			break;
		case 4:
			syscallExit(sf);
			break;
		default:break;
	}
}

void syscallWrite(struct StackFrame *sf) {
	switch(sf->ecx) { // file descriptor
		case 0:
			syscallPrint(sf);
			break; // for STD_OUT
		default:break;
	}
}

void syscallPrint(struct StackFrame *sf) {
	int sel = sf->ds; // segment selector for user data, need further modification
	char *str = (char*)sf->edx;
	int size = sf->ebx;
	int i = 0;
	int pos = 0;
	char character = 0;
	uint16_t data = 0;
	asm volatile("movw %0, %%es"::"m"(sel));
	for (i = 0; i < size; i++) {
		asm volatile("movb %%es:(%1), %0":"=r"(character):"r"(str+i));
		if(character == '\n') {
			displayRow++;
			displayCol=0;
			if(displayRow==25){
				displayRow=24;
				displayCol=0;
				scrollScreen();
			}
		}
		else {
			data = character | (0x0c << 8);
			pos = (80*displayRow+displayCol)*2;
			asm volatile("movw %0, (%1)"::"r"(data),"r"(pos+0xb8000));
			displayCol++;
			if(displayCol==80){
				displayRow++;
				displayCol=0;
				if(displayRow==25){
					displayRow=24;
					displayCol=0;
					scrollScreen();
				}
			}
		}

	}
	updateCursor(displayRow, displayCol);
	sf->eax=size;
	return;
}	

void memcpy(void* dst,void* src,size_t size){
	for(uint32_t j=0;j<size;j++){
		*(uint8_t*)(dst+j)=*(uint8_t*)(src+j);
	}
}

void syscallFork(struct StackFrame *sf){
	//TODO 完成fork
	// 如果没有空闲pcb，则fork失败，父进程返回-1，成功则子进程返回0，父进程返回子进程pid
	int pid = -1;
	for (int i = 1; i < MAX_PCB_NUM; i++)
		if (pcb[i].state == STATE_DEAD) {
			pid = i;
			sf->eax = pid;
			break;
		}
	if (pid == -1) {
		// pcb[current].regs.eax = -1; // 这里从syscall中返回后，eax的值会被系统调用前保存的值覆盖
		sf->eax = -1;
		return;
	}

	enableInterrupt();
	//TODO 拷贝地址空间
	memcpy((void *)((pid + 1) * 0x100000), (void *)((current + 1) * 0x100000), 0x100000);
	disableInterrupt();

	// 拷贝pcb，这部分代码给出了，请注意理解
	memcpy(&pcb[pid],&pcb[current],sizeof(ProcessTable));
	
	// pcb[current].regs.eax = pid;
	pcb[pid].regs.eax = 0;
	pcb[pid].regs.cs = USEL(1 + pid * 2);
	pcb[pid].regs.ds = USEL(2 + pid * 2);
	pcb[pid].regs.es = USEL(2 + pid * 2);
	pcb[pid].regs.fs = USEL(2 + pid * 2);
	pcb[pid].regs.gs = USEL(2 + pid * 2);
	pcb[pid].regs.ss = USEL(2 + pid * 2);
	pcb[pid].stackTop = (uint32_t)&(pcb[pid].regs);
	pcb[pid].state = STATE_RUNNABLE;
	pcb[pid].timeCount = 0;
	pcb[pid].sleepTime = 0;
	pcb[pid].pid = pid;
}	


void syscallExec(struct StackFrame *sf) {
	// TODO 完成exec
	uint32_t entry = 0;
	uint32_t secstart = sf->ecx;
	uint32_t secnum =  sf->edx;

	loadelf(secstart, secnum, (current + 1) * 0x100000, &entry);
	sf->eax = 0;
}


void syscallSleep(struct StackFrame *sf){
	//TODO:实现sleep
	pcb[current].sleepTime = sf->ecx;
	pcb[current].state = STATE_BLOCKED;
	asm volatile("int $0x20");
}	

void syscallExit(struct StackFrame *sf){
	//TODO 先设置成dead，然后用int 0x20进入调度
	pcb[current].state = STATE_DEAD;
	asm volatile("int $0x20");
}
