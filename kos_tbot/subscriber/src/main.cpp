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

    std::string const token("6061436734:AAH8WE92XUAlopJCYhwtuN5GvJqjv2eFQ7k");
    telegram::sender sender(token);
    telegram::listener::poll listener(sender);

    listener.set_callback_message([&](telegram::types::message const &message) {
      std::int64_t const admin_chat_id = 1046063063;
      std::vector<int64_t> allowed_user_ids = {1046063063};
      std::string dark = dark;
      std::string light = light;
      std::string const message_from(message.from ? message.from->first_name : "Unknown sender");
      std::string const reply(message_from + " sent \"" + *message.text + "\" to chat id " + std::to_string(message.chat.id));

      if(std::find(allowed_user_ids.begin(), allowed_user_ids.end(), message.chat.id) == allowed_user_ids.end()){
      	sender.send_message(message.chat.id, "I apologize, i am not allowed to answer you...\n Ask Konstantin to add you, maybe he will allow me to talk with you!");
      	sender.send_message(admin_chat_id, message_from + " tried to send \"" + *message.text + "\" to chat id " + std::to_string(message.chat.id));
      }
      else if(message.text && *message.text == "/turnon") {
      	gpiomain(dark);
	sender.send_message(message.chat.id, "Turning on...");
	sender.send_message(admin_chat_id, reply);
      }
      else if (*message.text == "/turnoff"){
      	gpiomain(light);
      	sender.send_message(message.chat.id, "Turning off...");
      	sender.send_message(admin_chat_id, reply);
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
