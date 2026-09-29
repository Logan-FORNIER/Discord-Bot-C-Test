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

	throw std::runtime_error("Environment variable " + var + " not set");
}

int main()
{

	const std::string TOKEN = get_env_var("BOT_TOKEN");
	const std::int64_t GUILD_ID = 1109223366629527603;

	/* Create bot cluster */
	dpp::cluster bot(TOKEN);

	/* Output simple log messages to stdout */
	bot.on_log(dpp::utility::cout_logger());

	/* Register slash command here in on_ready */
	bot.on_ready([&bot](const dpp::ready_t& event) {

		bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_game, "Discord Bot C++ by EL_IKO "));

		dpp::slashcommand ping("ping", "Ping Pong !", bot.me.id);
		dpp::slashcommand test("test", "Hello World !", bot.me.id);
		dpp::slashcommand presentation("presentation", "Génère une présentation de EL_IKO", bot.me.id);

		bot.guild_bulk_command_create({ping, test, presentation}, GUILD_ID);

	});

	bot.on_slashcommand([](const dpp::slashcommand_t& event) {

		if (event.command.get_command_name() == "test") {

			event.reply("Hello World !");

		}

		if (event.command.get_command_name() == "ping") {

			event.reply("Pong !");

		}

		if (event.command.get_command_name() == "presentation") {

			dpp::embed presentation = dpp::embed()
				.set_color(dpp::colors::red)
				.set_title("Présentation EL_IKO")
				.set_url("https://github.com/Logan-FORNIER")
				.add_field(

					"Test",
					"TESTTT",
					true

				);

			dpp::message msg(event.command.channel_id, presentation);

			event.reply(msg);

		}

	});

	/* Start the bot */
	bot.start(dpp::st_wait);

	return 0;
}
