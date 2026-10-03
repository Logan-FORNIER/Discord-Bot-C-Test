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
	const std::uint32_t intents = dpp::i_all_intents;

	dpp::cluster bot(TOKEN, intents);

	bot.on_log(dpp::utility::cout_logger());

	bot.on_ready([&bot, GUILD_ID](const dpp::ready_t& event) {

		bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_game, "Discord Bot C++ by EL_IKO "));

		dpp::slashcommand ping("ping", "Ping Pong !", bot.me.id);
		dpp::slashcommand test("test", "Hello World !", bot.me.id);
		dpp::slashcommand presentation("presentation", "Génère une présentation de EL_IKO", bot.me.id);
		dpp::slashcommand addrole("addrole", "Ajoute un role a un joueurs", bot.me.id);

		addrole.add_option(dpp::command_option(dpp::co_user, "user", "User to give role to", true));
		addrole.add_option(dpp::command_option(dpp::co_role, "role", "Role to give", true));

		bot.guild_bulk_command_create({ping, test, presentation, addrole}, GUILD_ID);

	});

	bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {

		if (event.command.get_command_name() == "test") {

			event.reply(dpp::message("Hello World !").set_flags(dpp::m_ephemeral));

		}

		if (event.command.get_command_name() == "ping") {

			event.reply("Pong !");

		}

		if (event.command.get_command_name() == "addrole") {

			dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));
			dpp::snowflake role_id = std::get<dpp::snowflake>(event.get_parameter("role"));

			dpp::guild* guild = dpp::find_guild(event.command.guild_id);

			if (guild) {

				dpp::role* target_role = dpp::find_role(role_id);
				dpp::guild_member bot_member = guild->members[bot.me.id];

				uint16_t highest_bot_position = 0;
				for (const auto& r_id : bot_member.get_roles()) {
					dpp::role* r = dpp::find_role(r_id);
					if (r_id && r->position > highest_bot_position) {

						highest_bot_position = r->position;

					}

				}

				if (target_role && target_role->position >= highest_bot_position) {

					event.reply("Impossible d'attribuer ce rôle : il est situé au dessus du rôle du bot");
					return;

				}
				bot.guild_member_add_role(event.command.guild_id, user_id, role_id);

				event.reply("Rôle ajouté avec succès");

			}

		}

		if (event.command.get_command_name() == "presentation") {

			dpp::embed presentation = dpp::embed()
				.set_color(dpp::colors::red)
				.set_title("Présentation EL_IKO")
				.set_url("https://github.com/Logan-FORNIER")
				.set_description("Bonjour, \n\nJe suis EL_IKO un petit dev indépendant apprenant le C++ au Gaming Campus.")
				.add_field(

					"Objectif",
					"Mon Objectif interne au sein du Gaming Campus est celle de devenir Developpeur Expert C++ spécialisé dans la création de jeux vidéo 3D et d'intelligence artificiel."

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
					"J'ai à mon actif différents projets personnel en cours ou fini tel que : \n\n- [The Maze ( Roblox Game )](<https://www.roblox.com/games/105209271404283/Le-labyrinthe>) \n- Bot Discord Test C++ \n- Site Web Portfolio"

				)
				.add_field(

					"Mes Réseaux",
					"- Github : [Logan-FORNIER](<https://github.com/Logan-FORNIER>) \n- Discord : <@1066421608199241738>"

				)
				.set_footer(

					dpp::embed_footer()
					.set_text("©2026 EL_IKO, Touts Droits Reservé")

				)
				.set_timestamp(time(0));

			event.reply(dpp::message().add_embed(presentation));

		}

	});

	bot.on_message_create([&bot](const dpp::message_create_t& event) {

		if (event.msg.author.is_bot()) return;

		std::string msg = event.msg.content;

		std::transform(msg.begin(), msg.end(), msg.begin(), [](unsigned char message) {

			return std::tolower(message);

		});

		if (msg == "bonjour" || msg == "salut" || msg == "bonjours" || msg == "saluts") {

			event.reply("Salut !");
			
			return;

		}

		if (msg == "bonsoir") {

			event.reply("Bonsoir !");
			return;

		}

	});

	bot.start(dpp::st_wait);

	return 0;
}
