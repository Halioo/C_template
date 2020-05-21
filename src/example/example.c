/**
 * @file example.c
 *
 * @brief This is an example of an active class
 *
 * @date April 2020
 *
 * @authors Thomas CRAVIC, Nathan LE GRANVALLET, Clément PUYBAREAU, Louis FROGER
 *
 * @version 1.0
 *
 * @copyright CCBY 4.0
 */

#include <pthread.h>
#include <mailbox.h>

#include "util.h"
#include "example.h"

/**
 * @brief Example instances counter used to have a unique queuename per thread
 */
static int exampleCounter = 0;

/* ----------------------- MAILBOX DEFINITIONS -----------------------*/

/**
 * @def Name of the task. Each instance will have this name,
 * followed by the number of the instance
 */
#define NAME_TASK "ExampleTask%d"

/**
 * @def Size of a task name
 */
#define SIZE_TASK_NAME 20


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
wrapperOf(Msg)

/**
 * @brief Structure of the Example object
 */
struct Example_t {
    pthread_t threadId; ///< Pthread identifier for the active function of the class.
    STATE state;        ///< Actual STATE of the STATE machine
    Msg msg;            ///< Structure used to pass parameters to the functions pointer.
    char nameTask[SIZE_TASK_NAME]; ///< Name of the task
    Mailbox * mb;

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
// TODO : write the events functions

void ExampleEventOne(Example * this, int param) {
    Msg msg = {
        .event = E_EXAMPLE1,
        .param2 = param
    };

    Wrapper wrapper;
    wrapper.data = msg;

    mailboxSendMsg(this->mb, wrapper.toString);
}

void ExampleEventTwo(Example * this, int param) {
    Msg msg = {
        .event = E_EXAMPLE2,
        .param2 = param
    };

    Wrapper wrapper;
    wrapper.data = msg;

    mailboxSendMsg(this->mb, wrapper.toString);
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

    while (this->state != S_DEATH) {
        mailboxReceive(this->mb, wrapper.toString); ///< Receiving an EVENT from the mailbox

        if (wrapper.data.event == E_KILL) { // If we received the stop EVENT, we do nothing and we change the STATE to death.
            this->state = S_DEATH;

        } else {
            action = stateMachine[this->state][wrapper.data.event].action;

            TRACE("Action %s\n", ACTION_toString[action])

            state = stateMachine[this->state][wrapper.data.event].nextState;
            TRACE("State %s\n", STATE_toString[state])

            if (state != S_FORGET) {
                this->msg = wrapper.data;
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
    this->mb = mailboxInit("Example", exampleCounter, sizeof(Msg));
    this->state = S_IDLE;

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
    Msg msg = { .event = E_KILL };

    Wrapper wrapper;
    wrapper.data = msg;

    mailboxSendStop(this->mb, wrapper.toString);
    TRACE("Waiting for the thread to terminate \n")

    int err = pthread_join(this->threadId, NULL);
    STOP_ON_ERROR(err != 0, "Error when waiting for the thread to end")

    return 0; // TODO: Handle the errors
}


int ExampleFree(Example * this) {
    // TODO : free the object with it particularities
    TRACE("ExampleFree function \n")
    mailboxClose(this->mb);

    free(this);

    return 0; // TODO: Handle the errors
}

