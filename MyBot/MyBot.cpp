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
	bot.on_ready([&bot, GUILD_ID](const dpp::ready_t& event) {

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
				.set_description("Bonjour, \n\nJe suis EL_IKO un petit dev indépendant apprenant le C++ au Gaming Campus.")
				.add_field(

					"Objectif",
					"Mon Objectif interne au sein du Gaming Campus est celle de devenir Developpeur Expert C++ spécialisé dans la création de jeux vidéo 3D et  d'intelligence artificiel."

				)
				.add_field(

					"Langage de Programmation Découvert",
					"- C++ \n- LuaU \n- Lua \n- Glua \n- Python \n- JavaScript \n- XML \n- Assembleur \n- Abap \n- SQL"

				)
				.add_field(

					"Statut",
					"[Première Année de G.Tech](<https://gamingcampus.fr/ecoles/ecole-developpeur-jeux-video-g-tech.html>]"

				)
				.add_field(

					"Projet Personnel",
					"J'ai a mon actif différent projet personnel en cours ou fini tel que : \n\n- [The Maze ( Roblox Game )](<https://www.roblox.com/games/105209271404283/Le-labyrinthe>) \n- Bot Discord Test C++ \n- Site Web Portfolio"

				)
				.add_field(

					"Mes Résaux",
					"- Github : [Logan-FORNIER](<https://github.com/Logan-FORNIER>) \n- Discord : <@1066421608199241738>"

				)
				.set_footer(

					dpp::embed_footer()
					.set_text("©2026 EL_IKO, Tout Droit Reservé")

				)
				.set_timestamp(time(0));
				
			event.reply(dpp::message().add_embed(presentation));

		}

	});

	/* Start the bot */
	bot.start(dpp::st_wait);

	return 0;
}
