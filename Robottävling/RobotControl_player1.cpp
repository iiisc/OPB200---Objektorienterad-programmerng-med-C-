#include "pch.h"
#include "RobotControl_player1.h"
#include <vector>

using namespace Player1;
Dir P1_dir = Dir::W;
int P1_roundCount = 0;
int P1_bananaCount = 5;
Command P1_lastCommand;

struct P1_proximityTreasure {
	bool treasure = false;
	int x = 1, y = 1;
};

struct P1_proximityRobot {
	bool robot = false;
	int x = 0, y = 0;
};

struct P1_square {
	bool banana = false;
	int posX = 0, posY = 0;
};

std::vector<P1_square> visitedSquares;

RobotControl::RobotControl()
{
	robot_bitmap_row = 2;          // Pick a unique monster icon for your robot!
	robot_bitmap_col = 13;
	team_name = L"PromenadSeger();";
}

bool P1_isVisited(Dir current, std::vector<P1_square>& vect) {
	bool visited = false;
	for (unsigned int i = 0; i < vect.size(); i++) {
		if (current == Dir::N) {
			if (vect[i].posX == 0 && vect[i].posY == -1)
				visited = true;
		}
		if (current == Dir::NE) {
			if (vect[i].posX == 1 && vect[i].posY == -1)
				visited = true;
		}
		if (current == Dir::E) {
			if (vect[i].posX == 1 && vect[i].posY == 0)
				visited = true;
		}
		if (current == Dir::SE) {
			if (vect[i].posX == 1 && vect[i].posY == 1)
				visited = true;
		}
		if (current == Dir::S) {
			if (vect[i].posX == 0 && vect[i].posY == 1)
				visited = true;
		}
		if (current == Dir::SW) {
			if (vect[i].posX == -1 && vect[i].posY == 1)
				visited = true;
		}
		if (current == Dir::W) {
			if (vect[i].posX == -1 && vect[i].posY == 0)
				visited = true;
		}
		if (current == Dir::NW) {
			if (vect[i].posX == -1 && vect[i].posY == -1)
				visited = true;
		}
	}
	return visited;
}

void P1_updateMemory(Dir current, Action action, std::vector<P1_square>& vect) {
	if (action == Action::STEP) {
		for (unsigned int i = 0; i < vect.size(); i++) {
			if (current == Dir::NW || current == Dir::W || current == Dir::SW)
				vect[i].posX++;
			if (current == Dir::NE || current == Dir::E || current == Dir::SE)
				vect[i].posX--;
			if (current == Dir::NW || current == Dir::N || current == Dir::NE)
				vect[i].posY++;
			if (current == Dir::SW || current == Dir::S || current == Dir::SE)
				vect[i].posY--;
		}
	}
	if (action == Action::PLACE_TRAP) {
		if (current == Dir::N) {
			if (P1_isVisited(current, vect)) { //Finns rutan med rätt koordinater redan, uppdatera rutan
				for (unsigned int i = 0; i < vect.size(); i++) {
					if (vect[i].posX == 0 && vect[i].posY == -1) {
						vect[i].banana = true;
					}
				}
			}
			else { //Finns rutan med rätt koordinater inte, skapa den med banana = true
				P1_square P1_bananaSquare;
				P1_bananaSquare.posX = 0;
				P1_bananaSquare.posY = -1;
				P1_bananaSquare.banana = true;
				vect.push_back(P1_bananaSquare);
			}
		}
		if (current == Dir::NE) {
			if (P1_isVisited(current, vect)) { //Finns rutan med rätt koordinater redan, uppdatera rutan
				for (unsigned int i = 0; i < vect.size(); i++) {
					if (vect[i].posX == 1 && vect[i].posY == -1) {
						vect[i].banana = true;
					}
				}
			}
			else { //Finns rutan med rätt koordinater inte, skapa den med banana = true
				P1_square P1_bananaSquare;
				P1_bananaSquare.posX = 1;
				P1_bananaSquare.posY = -1;
				P1_bananaSquare.banana = true;
				vect.push_back(P1_bananaSquare);
			}
		}
		if (current == Dir::E) {
			if (P1_isVisited(current, vect)) { //Finns rutan med rätt koordinater redan, uppdatera rutan
				for (unsigned int i = 0; i < vect.size(); i++) {
					if (vect[i].posX == 1 && vect[i].posY == 0) {
						vect[i].banana = true;
					}
				}
			}
			else { //Finns rutan med rätt koordinater inte, skapa den med banana = true
				P1_square P1_bananaSquare;
				P1_bananaSquare.posX = 1;
				P1_bananaSquare.posY = 0;
				P1_bananaSquare.banana = true;
				vect.push_back(P1_bananaSquare);
			}
		}
		if (current == Dir::SE) {
			if (P1_isVisited(current, vect)) { //Finns rutan med rätt koordinater redan, uppdatera rutan
				for (unsigned int i = 0; i < vect.size(); i++) {
					if (vect[i].posX == 1 && vect[i].posY == 1) {
						vect[i].banana = true;
					}
				}
			}
			else { //Finns rutan med rätt koordinater inte, skapa den med banana = true
				P1_square P1_bananaSquare;
				P1_bananaSquare.posX = 1;
				P1_bananaSquare.posY = 1;
				P1_bananaSquare.banana = true;
				vect.push_back(P1_bananaSquare);
			}
		}
		if (current == Dir::S) {
			if (P1_isVisited(current, vect)) { //Finns rutan med rätt koordinater redan, uppdatera rutan
				for (unsigned int i = 0; i < vect.size(); i++) {
					if (vect[i].posX == 0 && vect[i].posY == 1) {
						vect[i].banana = true;
					}
				}
			}
			else { //Finns rutan med rätt koordinater inte, skapa den med banana = true
				P1_square P1_bananaSquare;
				P1_bananaSquare.posX = 0;
				P1_bananaSquare.posY = 1;
				P1_bananaSquare.banana = true;
				vect.push_back(P1_bananaSquare);
			}
		}
		if (current == Dir::SW) {
			if (P1_isVisited(current, vect)) { //Finns rutan med rätt koordinater redan, uppdatera rutan
				for (unsigned int i = 0; i < vect.size(); i++) {
					if (vect[i].posX == -1 && vect[i].posY == 1) {
						vect[i].banana = true;
					}
				}
			}
			else { //Finns rutan med rätt koordinater inte, skapa den med banana = true
				P1_square P1_bananaSquare;
				P1_bananaSquare.posX = -1;
				P1_bananaSquare.posY = 1;
				P1_bananaSquare.banana = true;
				vect.push_back(P1_bananaSquare);
			}
		}
		if (current == Dir::W) {
			if (P1_isVisited(current, vect)) { //Finns rutan med rätt koordinater redan, uppdatera rutan
				for (unsigned int i = 0; i < vect.size(); i++) {
					if (vect[i].posX == -1 && vect[i].posY == 0) {
						vect[i].banana = true;
					}
				}
			}
			else { //Finns rutan med rätt koordinater inte, skapa den med banana = true
				P1_square P1_bananaSquare;
				P1_bananaSquare.posX = -1;
				P1_bananaSquare.posY = 0;
				P1_bananaSquare.banana = true;
				vect.push_back(P1_bananaSquare);
			}
		}
		if (current == Dir::NW) {
			if (P1_isVisited(current, vect)) { //Finns rutan med rätt koordinater redan, uppdatera rutan
				for (unsigned int i = 0; i < vect.size(); i++) {
					if (vect[i].posX == -1 && vect[i].posY == -1) {
						vect[i].banana = true;
					}
				}
			}
			else { //Finns rutan med rätt koordinater inte, skapa den med banana = true
				P1_square P1_bananaSquare;
				P1_bananaSquare.posX = -1;
				P1_bananaSquare.posY = -1;
				P1_bananaSquare.banana = true;
				vect.push_back(P1_bananaSquare);
			}
		}
	}
}

bool P1_isBanana(Dir current, std::vector<P1_square>& vect) {
	bool banana = false;
	for (unsigned int i = 0; i < vect.size(); i++) {
		if (current == Dir::N) {
			if (vect[i].posX == 0 && vect[i].posY == -1 && vect[i].banana)
				banana = true;
		}
		if (current == Dir::NE) {
			if (vect[i].posX == 1 && vect[i].posY == -1 && vect[i].banana)
				banana = true;
		}
		if (current == Dir::E) {
			if (vect[i].posX == 1 && vect[i].posY == 0 && vect[i].banana)
				banana = true;
		}
		if (current == Dir::SE) {
			if (vect[i].posX == 1 && vect[i].posY == 1 && vect[i].banana)
				banana = true;
		}
		if (current == Dir::S) {
			if (vect[i].posX == 0 && vect[i].posY == 1 && vect[i].banana)
				banana = true;
		}
		if (current == Dir::SW) {
			if (vect[i].posX == -1 && vect[i].posY == 1 && vect[i].banana)
				banana = true;
		}
		if (current == Dir::W) {
			if (vect[i].posX == -1 && vect[i].posY == 0 && vect[i].banana)
				banana = true;
		}
		if (current == Dir::NW) {
			if (vect[i].posX == -1 && vect[i].posY == -1 && vect[i].banana)
				banana = true;
		}
	}
	return banana;
}

bool P1_isBlocked(Dir currentDir, const Info& info) {
	if (currentDir == Dir::N) {
		if (info.neighbor_cells[0][1] == Cell_content::WALL
			|| info.neighbor_cells[0][1] == Cell_content::ROBOT
			|| info.neighbor_cells[0][1] == Cell_content::TRAP) {
			return true;
		}
	}
	if (currentDir == Dir::NE) {
		if (info.neighbor_cells[0][2] == Cell_content::WALL
			|| info.neighbor_cells[0][2] == Cell_content::ROBOT
			|| info.neighbor_cells[0][2] == Cell_content::TRAP) {
			return true;
		}
	}
	if (currentDir == Dir::E) {
		if (info.neighbor_cells[1][2] == Cell_content::WALL
			|| info.neighbor_cells[1][2] == Cell_content::ROBOT
			|| info.neighbor_cells[1][2] == Cell_content::TRAP) {
			return true;
		}
	}
	if (currentDir == Dir::SE) {
		if (info.neighbor_cells[2][2] == Cell_content::WALL
			|| info.neighbor_cells[2][2] == Cell_content::ROBOT
			|| info.neighbor_cells[2][2] == Cell_content::TRAP) {
			return true;
		}
	}
	if (currentDir == Dir::S) {
		if (info.neighbor_cells[2][1] == Cell_content::WALL
			|| info.neighbor_cells[2][1] == Cell_content::ROBOT
			|| info.neighbor_cells[2][1] == Cell_content::TRAP) {
			return true;
		}
	}
	if (currentDir == Dir::SW) {
		if (info.neighbor_cells[2][0] == Cell_content::WALL
			|| info.neighbor_cells[2][0] == Cell_content::ROBOT
			|| info.neighbor_cells[2][0] == Cell_content::TRAP) {
			return true;
		}
	}
	if (currentDir == Dir::W) {
		if (info.neighbor_cells[1][0] == Cell_content::WALL
			|| info.neighbor_cells[1][0] == Cell_content::ROBOT
			|| info.neighbor_cells[1][0] == Cell_content::TRAP) {
			return true;
		}
	}
	if (currentDir == Dir::NW) {
		if (info.neighbor_cells[0][0] == Cell_content::WALL
			|| info.neighbor_cells[0][0] == Cell_content::ROBOT
			|| info.neighbor_cells[0][0] == Cell_content::TRAP) {
			return true;
		}
	}
	return false;
}

P1_proximityRobot P1_isRobot(const Info& info) {
	P1_proximityRobot pr;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (info.neighbor_cells[i][j] == Cell_content::ROBOT && !(i == 1 && j == 1)) {
				pr.robot = true;
				pr.x = i;
				pr.y = j;
				return pr;
			}
		}
	}
	return pr;
}

Command P1_placeBanana(P1_proximityRobot pr, const Info& info) {
	if (pr.x == 0) {
		if (pr.y == 0) //NW
			return Command{ Action::PLACE_TRAP, Dir::N };
		if (pr.y == 1) //N
			return Command{ Action::PLACE_TRAP, Dir::NW };
		if (pr.y == 2) //NE
			return Command{ Action::PLACE_TRAP, Dir::E };
	}
	if (pr.x == 1) {
		if (pr.y == 0) //W
			return Command{ Action::PLACE_TRAP, Dir::NW };
		if (pr.y == 2) //E
			return Command{ Action::PLACE_TRAP, Dir::SE };
	}
	if (pr.x == 2) {
		if (pr.y == 0) //SW
			return Command{ Action::PLACE_TRAP, Dir::W };
		if (pr.y == 1) //S
			return Command{ Action::PLACE_TRAP, Dir::SW };
		if (pr.y == 2) //SE
			return Command{ Action::PLACE_TRAP, Dir::S };
	}
}

P1_proximityTreasure P1_isTreasure(const Info& info) { //Kollar om det finns treasure inom 3x3, returnerar struct med bool + riktning
	P1_proximityTreasure pt;
	pt.treasure = false;

	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			if (info.neighbor_cells[x][y] == Cell_content::TREASURE) {
				pt.treasure = true;
				pt.x = x;
				pt.y = y;
			}
		}
	}
	return pt;
}

Command P1_getTreasure(const Info& info, P1_proximityTreasure pt) {
	//Tar emot info om 3x3, samt "riktning" via x och y koordinat, returnerar kommando att gå dit
	if (pt.treasure) {
		if (pt.x == 0) {
			if (pt.y == 0)
				return Command{ Action::STEP, Dir::NW };
			if (pt.y == 1)
				return Command{ Action::STEP, Dir::N };
			if (pt.y == 2)
				return Command{ Action::STEP, Dir::NE };
		}
		if (pt.x == 1) {
			if (pt.y == 0)
				return Command{ Action::STEP, Dir::W };
			if (pt.y == 2)
				return Command{ Action::STEP, Dir::E };
		}
		if (pt.x == 2) {
			if (pt.y == 0)
				return Command{ Action::STEP, Dir::SW };
			if (pt.y == 1)
				return Command{ Action::STEP, Dir::S };
			if (pt.y == 2)
				return Command{ Action::STEP, Dir::SE };
		}
	}
}

Command P1_clockwiseSingle(Dir current) {
	if (current == Dir::N) {
		P1_dir = Dir::NE;
		return Command{ Action::STEP, Dir::NE };
	}
	if (current == Dir::NE) {
		P1_dir = Dir::E;
		return Command{ Action::STEP, Dir::E };
	}
	if (current == Dir::E) {
		P1_dir = Dir::SE;
		return Command{ Action::STEP, Dir::SE };
	}
	if (current == Dir::SE) {
		P1_dir = Dir::S;
		return Command{ Action::STEP, Dir::S };
	}
	if (current == Dir::S) {
		P1_dir = Dir::SW;
		return Command{ Action::STEP, Dir::SW };
	}
	if (current == Dir::SW) {
		P1_dir = Dir::W;
		return Command{ Action::STEP, Dir::W };
	}
	if (current == Dir::W) {
		P1_dir = Dir::NW;
		return Command{ Action::STEP, Dir::NW };
	}
	if (current == Dir::NW) {
		P1_dir = Dir::N;
		return Command{ Action::STEP, Dir::N };
	}
}

Command RobotControl::do_command(const Info& info)
{
	Command defaultCommand = Command{ Action::STEP,P1_dir }; //Default
	Command runCommand = defaultCommand; //RunCommand är det kommandot som uppdateras och sedan skickas

	P1_square currentSquare;
	P1_proximityTreasure pt;
	P1_proximityRobot pr;
	bool banankontakt = true;

	///**** HÄR STARTAR ROBOTLOGIKEN ****///

	// *** PRIO 5 *** // Byter riktning om rutan dit runCommand är inställt på finns i vektorn, dvs har besökts tidigare.
	int tryCounter = 0;
	while (P1_isVisited(runCommand.step_dir, visitedSquares) && tryCounter < 8) {
		runCommand = P1_clockwiseSingle(P1_dir);
		tryCounter++;
	}

	// *** PRIO 4 *** // Byter riktning om rutan dit runCommand har en banan man själv lagt ut
	while (P1_isBanana(runCommand.step_dir, visitedSquares)) {
		runCommand = P1_clockwiseSingle(P1_dir);
	}

	// *** PRIO 3 *** // Ändrar riktning om trap/wall/robot finns dit man vill gå
	while (P1_isBlocked(runCommand.step_dir, info)) {
		P1_roundCount++;
		if (P1_roundCount % 3 == 0 || P1_roundCount % 2 == 0) {
			if (P1_roundCount % 2 == 0) {
				runCommand = P1_clockwiseSingle(P1_dir);
				runCommand = P1_clockwiseSingle(P1_dir);
			}
			if (P1_roundCount % 3 == 0) {
				runCommand = P1_clockwiseSingle(P1_dir);
				runCommand = P1_clockwiseSingle(P1_dir);
				runCommand = P1_clockwiseSingle(P1_dir);
			}
			if (P1_roundCount % 5 == 0) {
				runCommand = P1_clockwiseSingle(P1_dir);
				runCommand = P1_clockwiseSingle(P1_dir);
				runCommand = P1_clockwiseSingle(P1_dir);
				runCommand = P1_clockwiseSingle(P1_dir);
			}
		}
	}

	// *** PRIO 2 *** // Placera ut banan nära motståndare
	pr = P1_isRobot(info);
	if (pr.robot && banankontakt && !(info.last_event == Event::COLLISION)) {
		if (P1_bananaCount > 0) {
			runCommand = P1_placeBanana(pr, info);
		}
	}

	// *** PRIO 1 *** // Finns skatt inom 3x3 ska den alltid plockas upp. 
	pt = P1_isTreasure(info);
	if (pt.treasure) {
		runCommand = P1_getTreasure(info, pt); //Overrides default
	}

	///**** ROBOTLOGIK SLUT ****///

	///**** HÄR STARTAR MINNESLOGIK ****///
	//uppdateMemory -- Uppdaterar alla squares i vektorn med nya koordinater
	P1_updateMemory(runCommand.step_dir, runCommand.action, visitedSquares);
	visitedSquares.push_back(currentSquare);
	if (runCommand.action == Action::PLACE_TRAP) {
		if (!P1_isBlocked(runCommand.step_dir, info)) {
			P1_bananaCount--;
		}
	}

	if (P1_lastCommand.action == Action::PLACE_TRAP && info.last_event == Event::COLLISION) {
		P1_bananaCount++;
	}
	///****   MINNESLOGIK SLUTAR    ****///
	P1_lastCommand = runCommand;
	return runCommand;
}
