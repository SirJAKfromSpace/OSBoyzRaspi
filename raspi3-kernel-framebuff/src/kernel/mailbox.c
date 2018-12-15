#include <kernel/mailbox.h>
#include <kernel/mem.h>
#include <common/stdlib.h>
mail_message_t mailbox_read(int channel) {
    mail_status_t stat;
    mail_message_t res;

    // Make sure that the message is from the right channel
    do {
        // Make sure there is mail to recieve
        do {
            stat = *MAIL0_STATUS;
        } while (stat.empty);

        // Get the message
        res = *MAIL0_READ;
    } while (res.channel != channel);

    return res;
}

void mailbox_send(mail_message_t msg, int channel) {
    mail_status_t stat;
    msg.channel = channel;


    // Make sure you can send mail
    do {
        stat = *MAIL0_STATUS;
    } while (stat.full);

    // send the message
    *MAIL0_WRITE = msg;
}

/**
 * returns the max of the size of the request and result buffer for each tag
 */
static uint32_t get_value_buffer_len(property_message_tag_t * tag) {
    switch(tag->proptag) {
        case FB_ALLOCATE_BUFFER:
        case FB_GET_PHYSICAL_DIMENSIONS:
        case FB_SET_PHYSICAL_DIMENSIONS:
        case FB_GET_VIRTUAL_DIMENSIONS:
        case FB_SET_VIRTUAL_DIMENSIONS:
            return 8;
        case FB_GET_BITS_PER_PIXEL:
        case FB_SET_BITS_PER_PIXEL:
        case FB_GET_BYTES_PER_ROW:
            return 4;
        case FB_RELESE_BUFFER:
        default:
            return 0;
    }
}

int send_messages(property_message_tag_t * tags) {
  puts("Entered send messages method");
    property_message_buffer_t * msg;
    mail_message_t mail;
    uint32_t bufsize = 0, i, len, bufpos;
puts("Entered send messages method 1\n");
    // Calculate the sizes of each tag
    for (i = 0; tags[i].proptag != NULL_TAG; i++) {
        bufsize += get_value_buffer_len(&tags[i]) + 3*sizeof(uint32_t);
    }
puts("Entered send messages method 2\n");
    // Add the buffer size, buffer request/response code and buffer end tag sizes
    bufsize += 3*sizeof(uint32_t);
puts("Entered send messages method 3\n");
    // buffer size must be 16 byte aligned
    bufsize += (bufsize % 16) ? 16 - (bufsize % 16) : 0;
puts("Entered send messages method 4\n");
    msg = kmalloc(bufsize);
    if (!msg){
        puts("Exited send messages method when !msg and returned -1 \n");
        return -1;
    }
puts("Entered send messages method 5 \n");
    msg->size = bufsize;
    msg->req_res_code = REQUEST;
puts("Entered send messages method 6 \n");
    // Copy the messages into the buffer
    for (i = 0, bufpos = 0; tags[i].proptag != NULL_TAG; i++) {
        len = get_value_buffer_len(&tags[i]);
        msg->tags[bufpos++] = tags[i].proptag;
        msg->tags[bufpos++] = len;
        msg->tags[bufpos++] = 0;
        memcpy(msg->tags+bufpos, &tags[i].value_buffer, len);
        bufpos += len/4;
    }
puts("Entered send messages method 7 \n");
    msg->tags[bufpos] = 0;
puts("Entered send messages method 8 \n");
    // Send the message
    mail.data = ((uint32_t)msg) >>4;
puts("Entered send messages method 9 \n");
    mailbox_send(mail, PROPERTY_CHANNEL);
    mail = mailbox_read(PROPERTY_CHANNEL);

puts("Entered send messages method 10 \n");
    if (msg->req_res_code == REQUEST) {
        kfree(msg);
        puts("Exited send messages method and returned 1 \n");
        return 1;
    }
    // Check the response code
    if (msg->req_res_code == RESPONSE_ERROR) {
        kfree(msg);
        puts("Exited send messages method and returned 2 \n");
        return 2;
    }

puts("Entered send messages method 11 \n");
    // Copy the tags back into the array
    for (i = 0, bufpos = 0; tags[i].proptag != NULL_TAG; i++) {
      puts("Entered send messages method for loop \n");
        len = get_value_buffer_len(&tags[i]);
        bufpos += 3; //skip over the tag bookkepping info
        memcpy(&tags[i].value_buffer, msg->tags+bufpos,len);
        bufpos += len/4;
    }
puts("Entered send messages method 12 \n");
    kfree(msg);
    puts("Exited send messages method and returned 0 \n");
    return 0;
    puts("Entered send messages method 13 \n");
}
