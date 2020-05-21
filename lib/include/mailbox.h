/**
 * @file mailbox.h
 *
 * @brief Mailbox class that allows to create multiple mailboxes based on mqueue library
 *
 * @date April 2020
 *
 * @authors Thomas CRAVIC, Nathan LE GRANVALLET, Clément PUYBAREAU, Louis FROGER, Guirec PLANCHAIS
 *
 * @copyright CCBY 4.0
 */

#ifndef MAILBOX_H
#define MAILBOX_H


/**
 * @def Name of the mailboxes. Each instance will have this name,
 * followed by the number of the instance
 */
#define NAME_MQ_BOX "/mbox%s%d"

/**
 * @def Size of the mailbox name
 */
#define SIZE_BOX_NAME 30

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


#include <stdio.h>
#include <pthread.h>
#include <mqueue.h>

#include "util.h"


/**
 * The mailbox structure
 */
typedef struct mailbox_t Mailbox;

/**
 * @brief Initializes the queue
 */
extern Mailbox * mailboxInit(char * objName, int objCounter, __syscall_slong_t maxMsgSize);

/**
 * @brief Destroys the queue
 */
extern void mailboxClose(Mailbox * this);

/**
 * @brief Sends a message to the queue
 *
 * @note This function is blocking if the queue is full
 * @param msg message
 */
extern void mailboxSendMsg(Mailbox * this, char * msg);

/**
 * @brief Sends a stop EVENT to the queue
 *
 * @note There is no specific content in the message, so there
 * is no need to specify any argument.
 */
extern void mailboxSendStop(Mailbox * this, char * msg);

/**
 * @brief Receives a message from the queue
 *
 * @note This function is blocking if the queue is empty
 * @param wrapper address of a message buffer
 */
extern void mailboxReceive(Mailbox * this, char * msg);


#endif //MAILBOX_H