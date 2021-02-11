#include <stdio.h>
#include <stdlib.h>     //rand
#include <stdbool.h>
#include <unistd.h>     //for fork()
#include <sys/wait.h>   //for pidwait()
#include <sys/mman.h>   //for mmap()
#include <stdlib.h>     //for exit()
#include <sys/types.h>  //for WEXITSTATUS
#include <semaphore.h>  //for sem_init()
#include <fcntl.h>      //for semaphore flags like O_CREAT

#define PF(args) do{printf args; fflush(0);} while(0)

static 
void exitQuit(char* msg, int retCode){
    PF(("\n!!Quit with code: %d,!!\n!!Reason: '%s'!!\n", retCode, msg));
    exit(retCode);
}

static 
int searchForPid( const int pid, const int childPids[], const size_t count ){
    for(int i = 0; i < count; i++){
        if(pid == childPids[i]){
            return i;
        }
    }
    return -1;
}

static 
void *allocateSharedMemBetweenProcesses( size_t size){
    void *shared_mem = mmap(
        NULL, size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1, 0 // for MAP_ANONYMOUS fd should be -1 and offeset 0
    );

    if(MAP_FAILED == shared_mem){
        exitQuit("mmap returned MAP_FAILED", EXIT_FAILURE);
    }

    return shared_mem;
}

#define NUM_CHILDREN (3)

typedef struct PidSemStatus_s {
    int   childPid   [NUM_CHILDREN];    
    int   childStatus[NUM_CHILDREN];  // child exit status (ie testsuite)
    sem_t childSem   [NUM_CHILDREN];  // child waits to start test
    sem_t parentSem  [NUM_CHILDREN];  // parent waits to know child ready to start test
    //"function ptr" childCode[NUM_CHILDREN] = { fn ptr 0, ... };
} PidSemStatus_t;

typedef struct SharedMem_s {
    PidSemStatus_t semData;
} SharedMem_t;

PidSemStatus_t* pSemData = NULL;


// ----------------------------------------------------------------------------
static int runChild(int child) {
  // start two way sync point
  PF(("child: %d informing parent I'm ready to rock and roll\n", child));
  sem_post( &( pSemData->parentSem[child] ) );
  PF(("child: %d wait on parent to let me rock and roll\n", child));
  sem_wait( &( pSemData->childSem[child] ) );
  // end   two way sync point

  int seconds = rand() % 5;
  PF(("child: %d boogie woogie.. my test starting..!!\n", child));
  sleep(seconds);
  PF(("child: %d ..  test done in %d sec!!\n", child, seconds));

  // run test .. call "function pointer"[child];
  // ...

  return EXIT_SUCCESS;
}


// ----------------------------------------------------------------------------
int main()
{
    SharedMem_t* shared_mem = allocateSharedMemBetweenProcesses(sizeof(SharedMem_t) );
    printf("-------- shared_mem OK--------\n");

    pSemData = &(shared_mem->semData);

    // //pshared is 1 for being shared between processes
    // if( sem_init(&shared_mem->semData, 1, 0) < 0 ){ 
    //     exitQuit("sem_init() failed", EXIT_FAILURE);  
    // }
    // printf("-------- sem_init() OK--------\n");

    printf("Parent: fork %d children & setup sems/stat arrays\n", NUM_CHILDREN);
    for (int child=0; child  < NUM_CHILDREN; child++) {
        // init childSem[child] to 0
        sem_t *pSem = &( pSemData->childSem[child] );
        if( sem_init(pSem, 1, 0) < 0 ){ 
            exitQuit("Child sem init failed", EXIT_FAILURE);  
        }
        printf("-------- sem_init() OK--------\n");

        // init parrentSem[child] to 0
        pSem =        &( pSemData->parentSem[child] );
        if( sem_init(pSem, 1, 0) < 0 ){ 
            exitQuit("Parent sem init failed", EXIT_FAILURE);  
        }

        int pid = fork();
        if( pid < 0 ){
            exitQuit("fork failed", EXIT_FAILURE);  
        }

        if (0 < pid) {
            PF(("Parent:  forked child %d has pid %d \n", child, pid));
            pSemData->childPid[child] = pid;
        } else {
            PF(("Child %d:  forked into existence. Waaa waaa..! \n", child));
            sleep(1); // let parent store pid in array
            srand(pSemData->childPid[child]*13);
            return runChild(child);
        } 
    }

    // ------------ tests takes place ------------

    // start of two way sync point
    PF(("Parent:  start of two way sync point ... \n"));
    for (int child=0; child  < NUM_CHILDREN; child++) {
        PF(("Parent:  wait for child %d to tell me it's ready to work \n", child));
        sem_wait( &( pSemData->parentSem[child] ) );
        PF(("Parent:  ..child %d ready \n", child));
    }
    for (int child=0; child  < NUM_CHILDREN; child++) {
        PF(("Parent:  letting child %d out to work \n", child));
        sem_post( &( pSemData->childSem[child] ) );
    }
    // end of two way sync point

    PF(("Parent:  All kids beavering away :) time for a brew.\n"));

    int overallStatus = 0;
    
    PF(("Parent: Wait for kids bring home the goods..\n"));
    for( int runningChildren = NUM_CHILDREN; 0 < runningChildren; ) {
        PF(("Parent: Wait for any child runner to end... zzzzz..\n"));
        // waits efficiently for ??any?? child status to change
        int status;
        int pid = wait(&status);
        if (pid < 0){
            exitQuit("Parent: error from wait for children", EXIT_FAILURE);
        } 

        PF(("Parent: someone with pid %d ended with status %d : Who is it?\n", pid, status));
        int updatedChildIdx = searchForPid(pid, pSemData->childPid, NUM_CHILDREN);
        if( updatedChildIdx < 0){
            exitQuit("unknown pid from wait for children", EXIT_FAILURE);
        }else{
            PF(("Parent: Ah, storing status for child idx %d\n", updatedChildIdx));
            pSemData->childStatus[updatedChildIdx] = status;
            overallStatus |= status;
            runningChildren -= 1;
        }
    }

    PF(("Parent: -------- all kids finished. Overall status: %d ------- \n", overallStatus));

    return overallStatus;
}

//  clang -g -pthread ./twoWaySync.c -o twoWaySync.exe
//  ./twoWaySync.exe 
// -------- shared_mem OK--------
// Parent: fork 3 children & setup sems/stat arrays
// -------- sem_init() OK--------
// Parent:  forked child 0 has pid 13238 
// -------- sem_init() OK--------
// Child 0:  forked into existence. Waaa waaa..! 
// Parent:  forked child 1 has pid 13239 
// -------- sem_init() OK--------
// Child 1:  forked into existence. Waaa waaa..! 
// Parent:  forked child 2 has pid 13240 
// Parent:  start of two way sync point ... 
// Parent:  wait for child 0 to tell me it's ready to work 
// Child 2:  forked into existence. Waaa waaa..! 
// child: 0 informing parent I'm ready to rock and roll
// child: 0 wait on parent to let me rock and roll
// Parent:  ..child 0 ready 
// Parent:  wait for child 1 to tell me it's ready to work 
// child: 1 informing parent I'm ready to rock and roll
// child: 1 wait on parent to let me rock and roll
// Parent:  ..child 1 ready 
// Parent:  wait for child 2 to tell me it's ready to work 
// child: 2 informing parent I'm ready to rock and roll
// child: 2 wait on parent to let me rock and roll
// Parent:  ..child 2 ready 
// Parent:  letting child 0 out to work 
// Parent:  letting child 1 out to work 
// child: 0 boogie woogie.. my test starting..!!
// Parent:  letting child 2 out to work 
// child: 1 boogie woogie.. my test starting..!!
// Parent:  All kids beavering away :) time for a brew.
// child: 2 boogie woogie.. my test starting..!!
// Parent: Wait for kids bring home the goods..
// Parent: Wait for any child runner to end... zzzzz..
// child: 2 ..  test done in 0 sec!!
// Parent: someone with pid 13240 ended with status 0 : Who is it?
// Parent: Ah, storing status for child idx 2
// Parent: Wait for any child runner to end... zzzzz..
// child: 0 ..  test done in 2 sec!!
// Parent: someone with pid 13238 ended with status 0 : Who is it?
// Parent: Ah, storing status for child idx 0
// Parent: Wait for any child runner to end... zzzzz..
// child: 1 ..  test done in 4 sec!!
// Parent: someone with pid 13239 ended with status 0 : Who is it?
// Parent: Ah, storing status for child idx 1
// Parent: -------- all kids finished. Overall status: 0 ------- 
