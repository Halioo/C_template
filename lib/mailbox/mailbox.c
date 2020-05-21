/**
 * @file mailbox.c
 *
 * @brief Mailbox class that allows to create multiple mailboxes based on mqueue library
 *
 * @date April 2020
 *
 * @authors TODO : Add author(s)
 *
 * @copyright CCBY 4.0
 * Based on templates written by Thomas CRAVIC, Nathan LE GRANVALLET, Clément PUYBAREAU, Louis FROGER
 */

#include "mailbox.h"
#include "errno.h"

struct mailbox_t {
    char queueName[SIZE_BOX_NAME];
    mqd_t mq;
    size_t mqSize;
};

/**
 * @brief Initializes the queue
 */
extern Mailbox * mailboxInit(char * objName, int objCounter, __syscall_slong_t maxMsgSize) {
    Mailbox * this = (Mailbox *) malloc(sizeof(Mailbox));
    sprintf(this->queueName, NAME_MQ_BOX, objName, objCounter);

    TRACE("[MAILBOX] Defined the Queue name : %s\n", this->queueName)

    TRACE("[MAILBOX] Oppening the mailbox %s\n", this->queueName)
    /* Destroying the mailbox if it already exists */
    errno = 0;
    int err = mq_unlink(this->queueName);
    if (err == -1) {
        if (errno == EACCES) {
            TRACE("ERROR : mq_unlink failed -> no permission to unlink the queue (continue)\n");
        } else if (errno == ENAMETOOLONG) {
            TRACE("ERROR : mq_unlink failed -> name too long (continue)\n");
        } else if (errno == ENOENT) {
            TRACE("ERROR : mq_unlink failed -> no message queue to unlink (continue)\n");
        } else {
            TRACE("ERROR : mq_unlink failed (exiting)\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Creating and opening the mailbox */

    /* Initializes the queue attributes */
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MQ_MAX_MESSAGES;		// Size of the queue
    attr.mq_msgsize = maxMsgSize;		// Max size of a message
    attr.mq_curmsgs = 0;

    // Creating the queue
    this->mqSize = maxMsgSize;
    errno = 0;
    this->mq = mq_open(this->queueName, O_CREAT | O_RDWR, 0600, &attr); // 600 = rw for owner and nothing else
    if(this->mq == -1){
        TRACE("ERROR : mq_open failed (exiting)\n");
        exit(EXIT_FAILURE);
    }
    return this;
}

/**
 * @brief Destroys the queue
 */
extern void mailboxClose(Mailbox * this) {
    errno = 0;
    int err = mq_close(this->mq);
    if (err == -1) {
        TRACE("ERROR : mq_close failed -> wrong mq descriptor (continue)\n");
    }
    /* Destruction of the queue */
    errno = 0;
    err = mq_unlink(this->queueName);
    if (err == -1) {
        if (errno == EACCES) {
            TRACE("ERROR : mq_unlink failed -> no permission to unlink the queue (continue)\n");
        } else if (errno == ENAMETOOLONG) {
            TRACE("ERROR : mq_unlink failed -> name too long (continue)\n");
        } else if (errno == ENOENT) {
            TRACE("ERROR : mq_unlink failed -> no message queue to unlink (continue)\n");
        } else {
            TRACE("ERROR : mq_unlink failed (exiting)\n");
            exit(EXIT_FAILURE);
        }
    }
    free(this);
}

/**
 * @brief Sends a message to the queue
 *
 * @note This function is blocking if the queue is full
 * @param msg message
 */
extern void mailboxSendMsg(Mailbox * this, char * msg) {
    errno = 0;
    ssize_t err = mq_send(this->mq, msg, this->mqSize, 0);
    if(err == -1){
        if(errno == EAGAIN){
            TRACE("ERROR : mq_send failed -> the queue is full (exiting)\n");
        }else if (errno == EMSGSIZE){
            TRACE("ERROR : mq_send failed -> msg length is greater than the mq_msgsize attribute of the queue (exiting)\n");
        }else if (errno == EBADF){
            TRACE("ERROR : mq_send failed -> wrong mq given or mq not opened for writing (exiting)\n");
        }else{
            TRACE("ERROR : mq_send failed (exiting)\n");
        }
        exit(EXIT_FAILURE);
    }else{
        TRACE("[MAILBOX] Sending message to the mailbox %s\n", this->queueName)
    }
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
    errno = 0;
    ssize_t err = mq_receive(this->mq, msg, this->mqSize, 0);
    if(err == -1) {
        if (errno == EMSGSIZE) {
            TRACE("ERROR : mq_receive failed -> msg length is less than the mq_msgsize attribute of the queue (exiting)\n");
        } else if (errno == EBADF) {
            TRACE("ERROR : mq_receive failed -> wrong mq given or mq or not opened for reading (exiting)\n");
        } else {
            TRACE("ERROR : mq_receive failed (exiting)\n");
        }
        exit((EXIT_FAILURE));
    }else {
        TRACE("[MAILBOX] Receiving a message from %s\n", this->queueName)
    }
}