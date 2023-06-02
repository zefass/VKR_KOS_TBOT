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
      std::string reddarkcmd = "reddark";
      std::string redlightcmd = "redlight";
      std::string yellowdarkcmd = "yellowdark";
      std::string yellowlightcmd = "yellowlight";
      std::string alldarkcmd = "alldark";
      std::string alllightcmd = "alllight";
      std::string const message_from(message.from ? message.from->first_name : "Unknown sender");
      std::string const reply(message_from + " sent \"" + *message.text + "\" to chat id " + std::to_string(message.chat.id));

      if(std::find(allowed_user_ids.begin(), allowed_user_ids.end(), message.chat.id) == allowed_user_ids.end()){
      	sender.send_message(message.chat.id, "I apologize, i am not allowed to answer you...\nAsk Konstantin to add you, maybe he will allow me to talk with you!");
      	sender.send_message(admin_chat_id, message_from + " tried to send \"" + *message.text + "\" to chat id " + std::to_string(message.chat.id));
      }
      else if(message.chat.id == admin_chat_id && *message.text == "/stop")
      {
      	sender.send_message(message.chat.id, "Gonna turn off now, bye!");
      	listener.stop();
      }
      else if(message.text && *message.text == "/help")
      {
      	sender.send_message(message.chat.id, "Here are current available requests:\n/turnon red - turn the red LED on\n/turnoff red - turn the red LED off\n/turnon yellow - turn the yellow LED on\n/turnoff yellow - turn the yellow LED off\n/turnon all - turn all LED on\n/turnoff all - turn all LED off");
      }
      else if(message.text && *message.text == "/turnon red") {
      	if(gpiomain(redlightcmd) == EXIT_SUCCESS) {
		sender.send_message(message.chat.id, "Turned on");
		sender.send_message(admin_chat_id, reply + " which succeded");
	}
	else {
		sender.send_message(message.chat.id, "Sorry, that didn't work for some reason, ask Konstantin about it...");
		sender.send_message(admin_chat_id, reply + " which failed");
	}
      }
      else if(message.text && *message.text == "/turnoff red") {
      	if(gpiomain(reddarkcmd) == EXIT_SUCCESS) {
		sender.send_message(message.chat.id, "Turned off");
		sender.send_message(admin_chat_id, reply + " which succeded");
	}
	else {
		sender.send_message(message.chat.id, "Sorry, that didn't work for some reason, ask Konstantin about it...");
		sender.send_message(admin_chat_id, reply + " which failed");
	}
      }
      else if(message.text && *message.text == "/turnon yellow") {
      	if(gpiomain(yellowlightcmd) == EXIT_SUCCESS) {
		sender.send_message(message.chat.id, "Turned on");
		sender.send_message(admin_chat_id, reply + " which succeded");
	}
	else {
		sender.send_message(message.chat.id, "Sorry, that didn't work for some reason, ask Konstantin about it...");
		sender.send_message(admin_chat_id, reply + " which failed");
	}
      }
      else if(message.text && *message.text == "/turnoff yellow") {
      	if(gpiomain(yellowdarkcmd) == EXIT_SUCCESS) {
		sender.send_message(message.chat.id, "Turned off");
		sender.send_message(admin_chat_id, reply + " which succeded");
	}
	else {
		sender.send_message(message.chat.id, "Sorry, that didn't work for some reason, ask Konstantin about it...");
		sender.send_message(admin_chat_id, reply + " which failed");
	}
      }
      else if(message.text && *message.text == "/turnon all") {
      	if(gpiomain(alllightcmd) == EXIT_SUCCESS) {
		sender.send_message(message.chat.id, "Turned on");
		sender.send_message(admin_chat_id, reply + " which succeded");
	}
	else {
		sender.send_message(message.chat.id, "Sorry, that didn't work for some reason, ask Konstantin about it...");
		sender.send_message(admin_chat_id, reply + " which failed");
	}
      }
      else if(message.text && *message.text == "/turnoff all") {
      	if(gpiomain(alldarkcmd) == EXIT_SUCCESS) {
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
