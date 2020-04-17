/**
 * @file example.c
 *
 * @brief This is an example of an active class
 * @date 13/04/2020
 * @author Thomas CRAVIC, Nathan LE GRANVALLET, Clément PUYBAREAU
 *
 * @copyright CCBY 4.0
 */

#include <pthread.h>
#include "../util.h"
#include "example.h"

/**
 * @brief Example instances counter used to have a unique queuename per thread
 */
static int exampleCounter = 0;

/* ----------------------- MAILBOX DEFINITIONS -----------------------*/

/**
 * @def Name of the mailboxes. Each instance will have this name,
 * followed by the number of the instance
 */
#define NAME_MQ_BOX "/mboxExample%d"

/**
 * @def Name of the task. Each instance will have this name,
 * followed by the number of the instance
 */
#define NAME_TASK "ExampleTask%d"


/**
 * @def Size of the mailbox name
 */
#define SIZE_BOX_NAME 20

/**
 * @def Size of a task name
 */
#define SIZE_TASK_NAME 20


/**
 * @def MAX_MESSAGE_LENGTH
 *
 * The maximum length of a message, including the null terminal character
 */
#define MAX_MESSAGE_LENGTH 1024

/**
 * @def MQ_MAX_MESSAGES
 *
 * The maximum number of message the queue can handle at one time
 *
 * By default, cannot be higher than 10, unless you change this
 * value into /proc/sys/fs/mqueue/msg_max
 */
#define MQ_MAX_MESSAGES (10)



/*----------------------- TYPE DEFINITIONS -----------------------*/

/**
 * @brief Enumeration of all the STATEs that can be taken by the STATE machine
 */
ENUM_DECL(STATE,
    S_FORGET,      ///< Nothing happens
    S_IDLE,        ///< Idle STATE
    S_RUNNING,     ///< Running STATE
    S_DEATH        ///< Transition STATE for stopping the STATE machine
)



/**
 * @brief Enumaration of all the possible ACTIONs called by the STATE machine
 */
ENUM_DECL(ACTION,
    A_NOP,                      ///< Nothing happens
    A_EXAMPLE1_FROM_RUNNING,    ///< ACTION called when passing from the running STATE to the example1 STATE
    A_EXAMPLE1_FROM_IDLE,       ///< ACTION called when passing from the idle STATE to the running STATE
    A_EXAMPLE2,                 ///< ACTION called when the Example2 EVENT happens
    A_KILL                      ///< Kills the STATE machine
)


/**
 * @brief Enumeration of all the possible EVENTs that triggers the STATE machine
 */
ENUM_DECL(EVENT,
    E_NOP,      ///< Do nothing
    E_EXAMPLE1, ///< EVENT example 1
    E_EXAMPLE2, ///< EVENT example 2
    E_KILL     ///< Kills the STATE machine
)


/**
 * @brief Transition structure of the STATE machine
 */
typedef struct {
    STATE nextState; ///< Next STATE of the STATE machine
    ACTION action;   ///< ACTION done before going in the next STATE
} Transition;


/**
 * @brief Structure of a message sent in the mailbox
 */
typedef struct {
    EVENT event; ///< EVENT sent in the message

    // TODO : Add here the parameters you want to be able to send through the mailbox. Do not remove the EVENT parameter.
    int param;  ///< Example of a possible parameter
    int param2; ///< Example of an other parameter
} Msg;


/**
 * @brief Wrapper enum. It is used to send EVENTs and parameters in a mailBox.
 */
typedef union {
    Msg msg; ///< Message sent, interpreted as a structure
    char toString[sizeof(Msg)]; ///< Message sent, interpreted as a char array
} Wrapper;


/**
 * @brief Structure of the Example object
 */
struct Example_t {
    pthread_t threadId; ///< Pthread identifier for the active function of the class.
    STATE state;        ///< Actual STATE of the STATE machine
    Msg msg;            ///< Structure used to pass parameters to the functions pointer.
    char queueName[SIZE_BOX_NAME]; ///< Name of the queue used to send EVENTs.
    char nameTask[SIZE_TASK_NAME]; ///< Name of the task
    mqd_t mq;


    // TODO : add here the instance variables you need to use.
    //Watchdog * wd; ///< Example of a watchdog implementation
    //int b; ///< Instance example variable
};


/*----------------------- STATIC FUNCTIONS PROTOTYPES -----------------------*/

/*------------- ACTION functions -------------*/
// TODO : put here all the ACTION functions prototypes
/**
 * @brief Function called when nothing needs to be done
 */
static void ActionNop(Example * this);


/**
 * @brief Changes the STATE of the STATE machine to S_DEATH
 */
static void ActionKill(Example * this);


/**
 * @brief Function called when there is the EVENT Example 1 and when the STATE is Idle
 */
static void ActionExample1FromIdle(Example * this);


/**
 * @brief Function called when there is the EVENT Example 1 and when the STATE is Running
 */
static void ActionExample1FromRunning(Example * this);


/**
 * @brief Function called when there is the EVENT Example 2 and when the STATE is Running
 */
static void ActionExample2(Example * this);


/*----------------------- STATE MACHINE DECLARATION -----------------------*/

/**
 * @def Function pointer used to call the ACTIONs of the STATE machine.
 */
typedef void (*ActionPtr)(Example*);

/**
 * @brief Function pointer array used to call the ACTIONs of the STATE machine.
 */
static const ActionPtr actionPtr[NB_ACTION] = { // TODO : add all the function pointers corresponding to the ACTION enum in the right order.
        &ActionNop,
        &ActionExample1FromRunning,
        &ActionExample1FromIdle,
        &ActionExample2,
        &ActionKill
};


/**
 * @brief STATE machine of the Example class
 */
static Transition stateMachine[NB_STATE][NB_EVENT] = { // TODO : fill the STATE machine
        [S_IDLE][E_EXAMPLE1]    = {S_RUNNING,	A_EXAMPLE1_FROM_IDLE},
        [S_RUNNING][E_EXAMPLE1] = {S_RUNNING, A_EXAMPLE1_FROM_RUNNING},
        [S_RUNNING][E_EXAMPLE2] = {S_IDLE, A_EXAMPLE2}
};


/* -------------- MAILBOX ------------- */

/**
 * @brief Initializes the queue
 */
static void mailboxInit(Example * this) {
    int err = sprintf(this->queueName, NAME_MQ_BOX, exampleCounter);
    TRACE("[MAILBOX] Defined the Queue name : %s\n", this->queueName)
    STOP_ON_ERROR(err < 0, "Error when setting the queueName")

    TRACE("[MAILBOX] Oppening the mailbox %s\n", this->queueName)
    /* Destroying the mailbox if it already exists */
    mq_unlink(this->queueName);

    /* Creating and opening the mailBox */

    /* Initializes the queue attributes */
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MQ_MAX_MESSAGES;		// Size of the queue
    attr.mq_msgsize = sizeof(Wrapper);		// Max size of a message
    attr.mq_curmsgs = 0;

    // Creating the queue
    mqd_t mq;
    mq = mq_open(this->queueName, O_CREAT | O_RDWR, 0600, &attr); // 600 = rw for owner and nothing else
    STOP_ON_ERROR(mq == -1, "Error when opening the mailbox %s", this->queueName)
    this->mq = mq;
}

/**
 * @brief Destroys the queue
 */
static void mailboxClose(Example *this) {
    int err = mq_close(this->mq);
    STOP_ON_ERROR(err != 0, "Error when closing the mqueue during initialisation")
    /* Destruction of the queue */
    err = mq_unlink(this->queueName);
    STOP_ON_ERROR(err == -1, "Error when unlinking the mailbox %s", this->queueName)
}

/**
 * @brief Sends a message to the queue
 *
 * @note This function is blocking if the queue is full
 * @param msg message
 */
static void mailboxSendMsg(Example *this, Msg msg) {
    TRACE("[MAILBOX] Sending message to the mailbox %s\n", this->queueName)

    Wrapper wrapper;
    wrapper.msg = msg;

    int err = mq_send(this->mq, wrapper.toString, sizeof(Wrapper), 0);
    STOP_ON_ERROR(err == -1, "Error when sending textOnly message")
}

/**
 * @brief Sends a stop EVENT to the queue
 *
 * @note There is no specific content in the message, so there
 * is no need to specify any argument.
 */
static void mailboxSendStop(Example *this) {
    TRACE("[MAILBOX] Sending stop EVENT to the queue %s\n", this->queueName)
    Msg msg = {.event = E_KILL};
    mailboxSendMsg(this, msg);
}

/**
 * @brief Receives a message from the queue
 *
 * @note This function is blocking if the queue is empty
 * @param wrapper address of a message buffer
 */
static void mailboxReceive(Example *this, Wrapper *wrapper) {
    TRACE("[MAILBOX] Receiving a message from %s\n", this->queueName)
    int err = mq_receive(this->mq, wrapper->toString, sizeof(Wrapper), 0);
    STOP_ON_ERROR(err == -1, "Error when receiving a message : ")
}


/* ----------------------- ACTIONS FUNCTIONS ----------------------- */

// TODO : Write all the ACTION functions

static void ActionExample1FromRunning(Example * this) {
    TRACE("[ActionEx1FromRunning] - %d\n", this->msg.param)
}


static void ActionExample1FromIdle(Example * this) {
    TRACE("[ActionEx1FromIdle] - %d\n", this->msg.param)
}


static void ActionExample2(Example * this) {
    TRACE("[ActionEx2] - %d\n", this->msg.param2)
}


static void ActionNop(Example * this) {
    TRACE("[ActionNOP]\n")
}

static void ActionKill(Example * this) {
    TRACE("[Action Kill]\n")
    this->state = S_DEATH;
}


/*----------------------- EVENT FUNCTIONS -----------------------*/
// TODO : write the EVENTs functions

void ExampleEventOne(Example * this, int param) {
    Msg message = {
            .event = E_EXAMPLE1,
            .param2 = param
    };

    mailboxSendMsg(this, message);
}

void ExampleEventTwo(Example * this, int param) {
    Msg message = {
            .event = E_EXAMPLE2,
            .param2 = param
    };

    mailboxSendMsg(this, message);
}

/*
extern void ExampleTimeout(Watchdog * wd, void * caller) {
    Msg message = {
        .EVENT = E_EXAMPLE2,
    };

    Example * this = caller;
    mailboxSendMsg(this, message);
}
*/


/* ----------------------- RUN FUNCTION ----------------------- */

/**
 * @brief Main running function of the Example class
 */
static void ExampleRun(Example * this) {
    ACTION action;
    STATE state;
    Wrapper wrapper;

    TRACE("RUN - Queue name : %s\n", this->queueName)

    while (this->state != S_DEATH) {
        mailboxReceive(this, &wrapper); ///< Receiving an EVENT from the mailbox

        if (wrapper.msg.event == E_KILL) { // If we received the stop EVENT, we do nothing and we change the STATE to death.
            this->state = S_DEATH;

        } else {
            action = stateMachine[this->state][wrapper.msg.event].action;

            TRACE("Action %s\n", ACTIONtoString[action])

            state = stateMachine[this->state][wrapper.msg.event].nextState;
            TRACE("State %s\n", STATEtoString[state])

            if (state != S_FORGET) {
                this->msg = wrapper.msg;
                actionPtr[action](this);
                this->state = state;
            }
        }
    }
}

/* ----------------------- NEW START STOP FREE -----------------------*/

Example * ExampleNew() {
    // TODO : initialize the object with it particularities
    exampleCounter ++; ///< Incrementing the instances counter.
    TRACE("ExampleNew function \n")
    Example * this = (Example *) malloc(sizeof(Example));
    this->state = S_IDLE;

    mailboxInit(this);
    //this->wd = WatchdogConstruct(1000, &ExampleTimeout, this); ///< Declaration of a watchdog.

    int err = sprintf(this->nameTask, NAME_TASK, exampleCounter);
    STOP_ON_ERROR(err < 0, "Error when setting the tasks name.")

    return this; // TODO: Handle the errors
}


int ExampleStart(Example * this) {
    // TODO : start the object with it particularities
    TRACE("ExampleStart function \n")
    int err = pthread_create(&(this->threadId), NULL, (void *) ExampleRun, this);
    STOP_ON_ERROR(err != 0, "Error when creating the thread")

    return 0; // TODO: Handle the errors
}


int ExampleStop(Example * this) {
    // TODO : stop the object with it particularities
    mailboxSendStop(this);
    TRACE("Waiting for the thread to terminate \n")

    int err = pthread_join(this->threadId, NULL);
    STOP_ON_ERROR(err != 0, "Error when waiting for the thread to end")

    return 0; // TODO: Handle the errors
}


int ExampleFree(Example * this) {
    // TODO : free the object with it particularities
    TRACE("ExampleFree function \n")
    mailboxClose(this);

    free(this);

    return 0; // TODO: Handle the errors
}

