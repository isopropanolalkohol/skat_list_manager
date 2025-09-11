CREATE TABLE players(playerID INT PRIMARY KEY AUTO_INCREMENT, name VARCHAR(30));
CREATE TABLE lists(listID INT PRIMARY KEY AUTO_INCREMENT, name VARCHAR(100));
CREATE TABLE players_lists(playerID INT, listID INT, PRIMARY KEY (playerID, listID), FOREIGN KEY (playerID) REFERENCES players(playerID), FOREIGN KEY (listID) REFERENCES lists(listID));
CREATE TABLE possibleGames(gameID INT PRIMARY KEY, score INT, typeName CHAR(12), peaks INT, modifierValue INT, modifierName CHAR(35));
CREATE TABLE played_games(gameID INT PRIMARY KEY AUTO_INCREMENT, playedGameID INT, playerID INT, listID INT, hasWon BOOL, playedAt DATETIME, FOREIGN KEY (playedGameID) REFERENCES possibleGames(gameID), FOREIGN KEY (playerID) REFERENCES players(playerID), FOREIGN KEY (listID) REFERENCES lists(listID));
