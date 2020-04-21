//
// Created by cleme on 19/04/2020.
//


#include "mailbox.h"

/**
 * @brief Initializes the queue
 */
extern Mailbox * mailboxInit(char * objName, int objCounter, __syscall_slong_t maxMsgSize) {
    Mailbox * this = (Mailbox *) malloc(sizeof(Mailbox));
    int err = sprintf(this->queueName, NAME_MQ_BOX, objName, objCounter);

    TRACE("[MAILBOX] Defined the Queue name : %s\n", this->queueName)
    STOP_ON_ERROR(err < 0, "Error when setting the queueName")

    TRACE("[MAILBOX] Oppening the mailbox %s\n", this->queueName)
    /* Destroying the mailbox if it already exists */
    mq_unlink(this->queueName);

    /* Creating and opening the mailbox */

    /* Initializes the queue attributes */
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MQ_MAX_MESSAGES;		// Size of the queue
    attr.mq_msgsize = maxMsgSize;		// Max size of a message
    attr.mq_curmsgs = 0;

    // Creating the queue
    this->mq = mq_open(this->queueName, O_CREAT | O_RDWR, 0600, &attr); // 600 = rw for owner and nothing else
    STOP_ON_ERROR(this->mq == -1, "Error when opening the mailbox %s", this->queueName)
    return this;
}

/**
 * @brief Destroys the queue
 */
extern void mailboxClose(Mailbox * this) {
    int err = mq_close(this->mq);
    STOP_ON_ERROR(err != 0, "Error when closing the mqueue during initialisation")
    /* Destruction of the queue */
    err = mq_unlink(this->queueName);
    STOP_ON_ERROR(err == -1, "Error when unlinking the mailbox %s", this->queueName)
    free(this);
}

/**
 * @brief Sends a message to the queue
 *
 * @note This function is blocking if the queue is full
 * @param msg message
 */
extern void mailboxSendMsg(Mailbox * this, char * msg) {
    TRACE("[MAILBOX] Sending message to the mailbox %s\n", this->queueName)
    int err = mq_send(this->mq, msg, sizeof(msg), 0);
    STOP_ON_ERROR(err == -1, "Error when sending textOnly message")
}

/**
 * @brief Sends a stop EVENT to the queue
 *
 * @note There is no specific content in the message, so there
 * is no need to specify any argument.
 */
extern void mailboxSendStop(Mailbox * this, char * msg) {
    TRACE("[MAILBOX] Sending stop event to the queue %s\n", this->queueName)
    mailboxSendMsg(this, msg);
}

/**
 * @brief Receives a message from the queue
 *
 * @note This function is blocking if the queue is empty
 * @param wrapper address of a message buffer
 */
extern void mailboxReceive(Mailbox * this, char * msg) {
    TRACE("[MAILBOX] Receiving a message from %s\n", this->queueName)
    int err = mq_receive(this->mq, msg, MAX_MESSAGE_LENGTH, 0);
    STOP_ON_ERROR(err == -1, "Error when receiving a message : ")
}
