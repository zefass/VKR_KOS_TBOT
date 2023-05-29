#include <exception>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <stdint.h>

#include <kos_net.h>
#include <gpio/gpio.h>
#include "general.h"
#include "gpioled.h"

#include <strings.h>
#include "libtelegram/libtelegram.h"


int main(void) try
{
    if (!wait_for_network())
    {
        throw std::runtime_error{"Error: Wait for network failed!"};
    }

    std::string const token("12343546");
    telegram::sender sender(token);
    telegram::listener::poll listener(sender);

    listener.set_callback_message([&](telegram::types::message const &message) {
      std::int64_t const admin_chat_id = 123;
      std::vector<int64_t> allowed_user_ids = {123};
      std::string darkcmd;
      std::string lightcmd;
      std::string const message_from(message.from ? message.from->first_name : "Unknown sender");
      std::string const reply(message_from + " sent \"" + *message.text + "\" to chat id " + std::to_string(message.chat.id));

      if(std::find(allowed_user_ids.begin(), allowed_user_ids.end(), message.chat.id) == allowed_user_ids.end()){
      	sender.send_message(message.chat.id, "I apologize, i am not allowed to answer you...\n Ask Konstantin to add you, maybe he will allow me to talk with you!");
      	sender.send_message(admin_chat_id, message_from + " tried to send \"" + *message.text + "\" to chat id " + std::to_string(message.chat.id));
      }
      else if(message.text && *message.text == "/turnon") {
      	if(gpiomain(lightcmd) == EXIT_SUCCESS) {
		sender.send_message(message.chat.id, "Turned on");
		sender.send_message(admin_chat_id, reply + " which succeded");
	}
	else {
		sender.send_message(message.chat.id, "Sorry, that didn't work for some reason, ask Konstantin about it...");
		sender.send_message(admin_chat_id, reply + " which failed");
	}
      }
      else if (*message.text == "/turnoff"){
      	if(gpiomain(darkcmd) == EXIT_SUCCESS) {
		sender.send_message(message.chat.id, "Turned off");
		sender.send_message(admin_chat_id, reply + " which succeded");
	}
	else {
		sender.send_message(message.chat.id, "Sorry, that didn't work for some reason, ask Konstantin about it...");
		sender.send_message(admin_chat_id, reply + " which failed");
	}
      }
    });
    listener.run();
    return EXIT_SUCCESS;
}
catch (std::exception& exc)
{
    std::cerr << app::AppTag << exc.what() << std::endl;
    return EXIT_FAILURE;
}
