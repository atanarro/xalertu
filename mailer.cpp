#include <libesmtp.h>

int main() {

  smtp_session_t my_smtp_session = smtp_create_session();
  smtp_message_t my_smtp_message = smtp_add_message(my_smtp_session);
  smtp_set_server(my_smtp_session, "smtp.gmail.com:25");
  smtp_add_recipient(my_smtp_message, "tanarro@gmail.com");

}
