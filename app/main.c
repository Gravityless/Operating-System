#include "lib.h"
#include "types.h"
#define N 5

int uEntry(void) {

	// int ret = 0;
	
	// 测试scanf	
	// int dec = 0;
	// int hex = 0;
	// char str[6];
	// char cha = 0;
	// while(1){
	// 	printf("Input:\"Test %%c Test %%6s %%d %%x\"\n");
	// 	ret = scanf("%c %6s %d %x", &cha, str, &dec, &hex);
	// 	printf("Ret: %d; %c, %s, %d, %x.\n", ret, cha, str, dec, hex);
	// 	if (ret == 4)
	// 		break;
	// }
	
	// 测试信号量
	// int i = 4;
	// sem_t sem;
	// printf("Father Process: Semaphore Initializing.\n");
	// ret = sem_init(&sem, 0);
	// if (ret == -1) {
	// 	printf("Father Process: Semaphore Initializing Failed.\n");
	// 	exit();
	// }

	// ret = fork();
	// if (ret == 0) {
	// 	while( i != 0) {
	// 		i --;
	// 		printf("Child Process: Semaphore Waiting.\n");
	// 		sem_wait(&sem);
	// 		printf("Child Process: In Critical Area.\n");
	// 	}
	// 	printf("Child Process: Semaphore Destroying.\n");
	// 	sem_destroy(&sem);
	// 	exit();
	// }
	// else if (ret != -1) {
	// 	while( i != 0) {
	// 		i --;
	// 		printf("Father Process: Sleeping.\n");
	// 		sleep(128);
	// 		printf("Father Process: Semaphore Posting.\n");
	// 		sem_post(&sem);
	// 	}
	// 	printf("Father Process: Semaphore Destroying.\n");
	// 	sem_destroy(&sem);
	// 	exit();
	// }

	// For lab4.3
	// TODO: You need to design and test the philosopher problem.
	// Note that you can create your own functions.
	// Requirements are demonstrated in the guide.
	
	//哲学家
	int id = 0;
	int ret = 0;
	
	sem_t sem[N];
	for (int i = 0; i < 5; i++) {
		if (sem_init(&sem[i], 1) == -1) {
			printf("Semaphore Initializing Failed\n");
			break;
		}
	}
	printf("Semaphore Initializing Succeed\n");

	ret = fork();
	printf("ret = %d, id = %d\n", ret, id);
	while (ret != 0 && id < 4) {
		ret = fork();
		id++;
		printf("ret = %d, id = %d\n", ret, id);
	}
	if(ret != 0 && id == 4) exit();
	printf("Philosopher %d: come\n", id);

	sleep(500);

	// while (1) {
	// 	printf("Philosopher %d: think\n", id);
		
	// 	if ((id & 1) == 1) {
	// 		sem_wait(&sem[(id + 1) % N]);
	// 		printf("Philosopher %d: pick up %d\n", id, (id + 1) % N);
	// 		sem_wait(&sem[id]);
	// 		printf("Philosopher %d: pick up %d\n", id, id % N);
	// 	} else {
	// 		sem_wait(&sem[id]);
	// 		printf("Philosopher %d: pick up %d\n", id, id % N);
	// 		sem_wait(&sem[(id + 1) % N]);
	// 		printf("Philosopher %d: pick up %d\n", id, (id + 1) % N);
	// 	}
	// 	printf("Philosopher %d: eat\n", id);
		
	// 	sem_post(&sem[(id + 1) % N]);
	// 	printf("Philosopher %d: release %d\n", id, (id + 1) % N);
	// 	sem_post(&sem[id]);
	// 	printf("Philosopher %d: release %d\n", id, id % N);
	// }

	// sem_t mutex;
	// sem_init(&mutex, 1);

	// while (1) {
	// 	printf("Philosopher %d: think\n", id);

	// 	sem_wait(&mutex);
	// 	sem_wait(&sem[id]);
	// 	sem_wait(&sem[(id + 1) % N]);

	// 	printf("Philosopher %d: eat\n", id);

	// 	sem_post(&sem[id]);
	// 	sem_post(&sem[(id + 1) % N]);
	// 	sem_post(&mutex);
	// }

	//生产者消费者问题

	//读者写者问题
	

	exit();
	return 0;
}
