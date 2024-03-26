/*
* @description Fin 添加注释
* @author  CPT103_CW3_ICSGroup1
* @date  29.11.2022
* @version b0.1
*/

SET FOREIGN_KEY_CHECKS = 0;
drop table if exists drink_type;
drop table if exists drink;
drop table if exists ingredient;
drop table if exists packaging_design;
drop table if exists vending_machine;
drop table if exists team;
SET FOREIGN_KEY_CHECKS = 1;

CREATE TABLE drink_type (
    drinkName VARCHAR(255) PRIMARY KEY, /*The identifying column of drink name*/
    Total_Calories INT DEFAULT 0, /*Default to 0 if not filled in*/
    Saturated_Fat INT DEFAULT 0,
    Trans_fatty_acid INT DEFAULT 0,
    Cholesterol INT DEFAULT 0,
    Carbohydrate INT DEFAULT 0,
    Protein INT DEFAULT 0,
    Vitamin_D INT DEFAULT 0,
    Calcium INT DEFAULT 0,
    Magnesium INT DEFAULT 0,
    Potassium INT DEFAULT 0
);
CREATE TABLE team(
    team_name VARCHAR(100) PRIMARY KEY, /*The identifying column of team*/
    phoneNum INT UNIQUE NOT NULL, /*Each team needs to have a different phone number*/
    trunkId INT NOT NULL
);
CREATE TABLE vending_machine (
    machineId VARCHAR(50) PRIMARY KEY, /*The identifying column of vending machine*/
    location_machine VARCHAR(100) UNIQUE NOT NULL, /*The location of the machine must be different*/
    team_name VARCHAR(100) NOT NULL,
    CONSTRAINT fk_vendingmachine_team FOREIGN KEY (team_name) REFERENCES team(team_name)
);
CREATE TABLE packaging_design(
    packaging_design_name VARCHAR(255) PRIMARY KEY, /*The identifying column of packaging design*/
    main_material VARCHAR(100) NOT NULL, 
    milliltres INT NOT NULL CHECK(milliltres>0), 
    CONSTRAINT UQ_packaging_design UNIQUE(packaging_design_name,main_material,milliltres) /*to avoid duplication of combinations of packaging names, main materials, and milliltres*/
);
CREATE TABLE drink (
    drinkId VARCHAR(50) PRIMARY KEY, /*The identifying column of drink*/
    production_date DATE NOT NULL,
    expiry_date DATE NOT NULL,
    drinkName VARCHAR(255) NOT NULL,
    packaging_design_name VARCHAR(255) NOT NULL,
    machineId VARCHAR(50),              /*It can be null,because drinks are not always in the machine*/
    CONSTRAINT fk_drink_drinktype FOREIGN KEY(drinkName) REFERENCES drink_type(drinkName),
    CONSTRAINT fk_drink_packagingdesign FOREIGN KEY(packaging_design_name) REFERENCES packaging_design(packaging_design_name),
    CONSTRAINT fk_drink_vendingmachine FOREIGN KEY (machineId) REFERENCES vending_machine(machineId)
);
CREATE TABLE ingredient (
    drinkName VARCHAR(255) NOT NULL,
    ingredients VARCHAR(255) NOT NULL,
    CONSTRAINT UQ_ingredient UNIQUE(drinkName,ingredients), /*To avoid repetition on combinations of drinkName and ingredients*/
    CONSTRAINT fk_ingredient_drink FOREIGN KEY(drinkName) REFERENCES drink_type(drinkName) 
);

