#define _CRT_SECURE_NO_WARNINGS

#include "MyBot.h"
#include <dpp/dpp.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <stdexcept>


std::string get_env_var(const std::string& var)
{
	const char* val = std::getenv(var.c_str());
	if (val != nullptr) {
		return std::string(val);
	}

	std::ifstream file(".env");
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			if (!line.empty() && line.back() == '\r') {
				line.pop_back();
			}

			size_t pos = line.find('=');
			if (pos != std::string::npos) {
				std::string key = line.substr(0, pos);
				std::string value = line.substr(pos + 1);
				if (key == var) {
					return value;
				}
			}
		}
	}
	throw std::runtime_error("Environment variable " + var + " not set");
}

int main()
{

	const std::string TOKEN = get_env_var("BOT_TOKEN");

	/* Create bot cluster */
	dpp::cluster bot(TOKEN);

	/* Output simple log messages to stdout */
	bot.on_log(dpp::utility::cout_logger());

	/* Register slash command here in on_ready */
	bot.on_ready([&bot](const dpp::ready_t& event) {
		/* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
		if (dpp::run_once<struct register_bot_commands>()) {
			std::vector<dpp::slashcommand> commands {
				{ "ping", "Ping pong!", bot.me.id }
			};

			bot.global_bulk_command_create(commands);
		}
	});

	/* Handle slash command with the most recent addition to D++ features, coroutines! */
	bot.on_slashcommand([](const dpp::slashcommand_t& event) -> dpp::task<void> {
		if (event.command.get_command_name() == "ping") {
			co_await event.co_reply("Pong!");
		}
		co_return;
	});

	/* Start the bot */
	bot.start(dpp::st_wait);

	return 0;
}
