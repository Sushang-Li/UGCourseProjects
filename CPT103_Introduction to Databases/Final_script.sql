/*
*TASK1
*/

/*
* @description Fin add comments
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

/*
*TASK2
*/

/*
*@author Duanru Li
*@id 2142112
*/

SET FOREIGN_KEY_CHECKS = 0;
drop table if exists basic_info;
drop table if exists qualification;
drop table if exists inspection_items;
SET FOREIGN_KEY_CHECKS = 1;

CREATE TABLE basic_info(
    drinkName VARCHAR(255) NOT NULL,
    packaging_design_name VARCHAR(255) NOT NULL,
    batch_num VARCHAR(50) PRIMARY KEY,
    inspector VARCHAR(50),
    inspect_date DATE, 
    CONSTRAINT fk_basicinfo_drink FOREIGN KEY (drinkName) REFERENCES drink(drinkName)
);    

CREATE TABLE qualification(
    batch_num VARCHAR(50) PRIMARY KEY,
    production_quantity INT NOT NULL,
    sample_quantity INT,
    qualified_quantity INT,
    CONSTRAINT fk_qualification_basicinfo FOREIGN KEY (batch_num) REFERENCES basic_info(batch_num)
);    

CREATE TABLE inspection_items(
    sample_id VARCHAR(50) PRIMARY KEY,
    batch_num VARCHAR(50) NOT NULL,
    colour VARCHAR(50) DEFAULT 'qualified',
    smell VARCHAR(50) DEFAULT 'qualified',
    impurity VARCHAR(50) DEFAULT 'qualified',
    net_weight VARCHAR(50) DEFAULT 'qualified',
    airtightness VARCHAR(50) DEFAULT 'qualified',
    qualification_judgment VARCHAR(50) DEFAULT 'qualified',
    CONSTRAINT fk_inspectionitems_basicinfo FOREIGN KEY (batch_num) REFERENCES basic_info(batch_num)
);    

/*
*@author Rongjie Mao
*@id 2141522
*/

DROP TABLE IF EXISTS company;
CREATE TABLE company (
  SpecialtyStores varchar(255) NOT NULL,
  DepartmentName varchar(50) NOT NULL,
  City varchar(50) NOT NULL,
  EmployeesID int NOT NULL,
  PRIMARY KEY (SpecialtyStores)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

DROP TABLE IF EXISTS department;
CREATE TABLE department(
  Position varchar(50) NOT NULL,
  Performance varchar(255) NOT NULL,
  KEY Position (Position)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

DROP TABLE IF EXISTS employees;
CREATE TABLE employees (
  Position varchar(50) NOT NULL,
  Onboarding_time date NOT NULL,
  Salary int NOT NULL,
  EmployeesName varchar(255) NOT NULL,
  SpecialtyStores varchar(255) NOT NULL,
  EmployeesID int NOT NULL,
  PRIMARY KEY (Position),
  KEY SpecialtyStores (SpecialtyStores),
  KEY Salary (Salary)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

DROP TABLE IF EXISTS income;
CREATE TABLE income (
  Salary int NOT NULL,
  Bonus int DEFAULT 0,
  Position varchar(50) NOT NULL,
  PRIMARY KEY (Salary)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

DROP TABLE IF EXISTS sales_department;
CREATE TABLE sales_department (
  SpecialtyStores varchar(255) NOT NULL,
  team_name varchar(100) NOT NULL,
  SalesmanName varchar(255) NOT NULL,
  KEY team_name (team_name),
  KEY SpecialtyStores (SpecialtyStores)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

ALTER TABLE `sales_department` CHANGE `team_name` `team_name` VARCHAR(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL;

ALTER TABLE `department`
  ADD CONSTRAINT `department_ibfk_1` FOREIGN KEY (`Position`) REFERENCES `employees` (`Position`);
ALTER TABLE `employees`
  ADD CONSTRAINT `employees_ibfk_1` FOREIGN KEY (`SpecialtyStores`) REFERENCES `company` (`SpecialtyStores`),
  ADD CONSTRAINT `employees_ibfk_2` FOREIGN KEY (`Salary`) REFERENCES `income` (`Salary`);
ALTER TABLE `sales_department`
  ADD  FOREIGN KEY (`team_name`) REFERENCES `team` (`team_name`),
  ADD  FOREIGN KEY (`SpecialtyStores`) REFERENCES `company` (`SpecialtyStores`);

/*
*@author Mingzhe Li
*@id 2145668
*/

alter table vending_machine add type varchar(50) ;

create table vending_mahine_dealerInfo (
    id int auto_increment primary key ,
    machineId varchar(50),
    dealerID varchar(50)
) comment 'Dealer-agent intermediate table';

create table dealer_brandInfo(
dealerId varchar(50) not null ,
agentBrandName varchar(50) not null ,
vendingMachinePrice varchar(50)not null ,
vendingType varchar(50) not null ,
maximum_capacity varchar(255) not null ,
CONSTRAINT UQ_brand UNIQUE(dealerId,agentBrandName,vendingMachinePrice,vendingType)
)comment 'Agent information';

alter table vending_mahine_dealerInfo ADD CONSTRAINT fk_delaer_brand foreign key (dealerID) references dealer_brandInfo(dealerId);
alter table vending_mahine_dealerInfo ADD CONSTRAINT fk_machine_dealer foreign key (machineId)references vending_machine(machineId);

/*
*@author Xinyao Chen
*@id 2142261
*/

drop table if exists order_list;
drop table if exists warehouse;

CREATE TABLE warehouse (
    warehouse_id VARCHAR(50) PRIMARY KEY,
    warehouse_name VARCHAR(50) UNIQUE NOT NULL,
    warehouse_address VARCHAR(50) UNIQUE NOT NULL,
    square_meter INT NOT NULL
);

ALTER TABLE drink  ADD  warehouse_id VARCHAR (50) ;  -- Add a column to the original table `drink`.
ALTER TABLE drink ADD CONSTRAINT FK_DRINK_WAREHOUSE FOREIGN KEY (warehouse_id) REFERENCES warehouse (warehouse_id) ON UPDATE CASCADE ON DELETE NO ACTION;

CREATE TABLE order_list (
    order_id VARCHAR (50) PRIMARY KEY, 
    location_machine VARCHAR(100) NOT NULL, 
    warehouse_name VARCHAR(50) NOT NULL, 
    note VARCHAR(200),
    create_time DATETIME NOT NULL,
    complete_time DATETIME,
CONSTRAINT fk_warehouse_order_list FOREIGN KEY (warehouse_name) REFERENCES warehouse (warehouse_name),
CONSTRAINT fk_warehouse_vending_machine FOREIGN KEY (location_machine) REFERENCES vending_machine (location_machine)
);

/*
*@author Mingyuan Li
*@id 2145618
*/

SET FOREIGN_KEY_CHECKS = 0;
drop table if exists main_material_storage;
drop table if exists material_supplier;
drop table if exists material_supply;
drop table if exists material_order;
SET FOREIGN_KEY_CHECKS = 1;

CREATE TABLE main_material_storage(
    main_material VARCHAR(100) PRIMARY KEY, /*The name of the main materials. Change this column as soon as material is added or removed*/
    remain_amount INT DEFAULT 0 CHECK(remain_amount>=0), /*Remaining amount cannot be less than 0*/
    average_consumption INT NOT NULL CHECK(average_consumption>=0) /*Average consumption cannot be less than 0*/
);
CREATE TABLE material_supplier(
    supplierId INT PRIMARY KEY, /*The identifying column of suppliers*/
    supplierName VARCHAR(255) NOT NULL, /*It can be repeated because the same supplier has branches in different cities*/
    supplier_email VARCHAR(100) UNIQUE NOT NULL, /*It must be unique to ensure accurate contact */
    city VARCHAR(20) NOT NULL, /*Same as column supplierName*/
    CONSTRAINT UQ_materialsupplier UNIQUE (supplierName,city) /*Ensure there are no duplicate supplierName and city combinations*/
);
CREATE TABLE material_supply(
    supplierId INT NOT NULL, 
    main_material VARCHAR(100) NOT NULL,
    cost_perKg INT NOT NULL CHECK(cost_perKg>0), /*Cost must greater than 0*/
    transport_time INT NOT NULL CHECK(transport_time>=0), /*Transport time cannot be less than 0*/
    CONSTRAINT pk_material_supply PRIMARY KEY(supplierId,main_material), /*The composite key of suppliers and main_material*/
    CONSTRAINT fk_materialsupply_mainmaterialstorage FOREIGN KEY (main_material) REFERENCES main_material_storage(main_material),
    CONSTRAINT fk_materialsupply_materialsupplier FOREIGN KEY (supplierId) REFERENCES material_supplier(supplierId)
);
CREATE TABLE material_order(
    orderId INT PRIMARY KEY, /*Unique order number per order*/
    supplierId INT NOT NULL, 
    main_material VARCHAR(100) NOT NULL,
    order_amount INT NOT NULL CHECK(order_amount>0), /*Amount must greater than 0*/
    order_status VARCHAR(255) NOT NULL, /*Show the current status of the order*/
    CONSTRAINT fk_materialorder_materialsupply FOREIGN KEY (supplierId,main_material) REFERENCES material_supply(supplierId,main_material)
); 
ALTER TABLE packaging_design
ADD CONSTRAINT fk_packagingdesign_mainmaterialstorage FOREIGN KEY (main_material) REFERENCES main_material_storage(main_material) ;/*add foreign key*/

/*
*@author Dongling Wang
*@id 2143873
*/

SET FOREIGN_KEY_CHECKS = 0;
drop table if exists ingredients_brand;
drop table if exists ingredients_manufacturer;
drop table if exists sweetener;
drop table if exists preservative;
drop table if exists drink_factory;
SET FOREIGN_KEY_CHECKS = 1;

CREATE TABLE ingredients_manufacturer (
    brand varchar(100) PRIMARY KEY,
    manufacturer varchar(255)
);

CREATE TABLE all_ingredient (
    ingredient VARCHAR(255) PRIMARY KEY /*Intermediate table*/
);

CREATE TABLE ingredients_brand (
    ingredients VARCHAR(255) ,
    brand VARCHAR(100) NOT NULL,
    `cost (per t)` DOUBLE,
    CONSTRAINT fk_brand FOREIGN KEY (brand) REFERENCES ingredients_manufacturer (brand),
    CONSTRAINT fk_brand_ingredients FOREIGN KEY (ingredients) REFERENCES all_ingredient (ingredient)
);

ALTER TABLE ingredient
ADD CONSTRAINT pk_raw PRIMARY KEY (ingredients),
ADD CONSTRAINT fk_ri FOREIGN KEY (ingredients) REFERENCES all_ingredient (ingredient);

CREATE TABLE drink_sugar (
    drinkName VARCHAR(255),
    sugar_name VARCHAR(255), /*The sugars in the ingredients are used as a separate table*/
    relative_sweetness DOUBLE DEFAULT 0,
    CONSTRAINT fk_drink_sweetener FOREIGN KEY (drinkName) REFERENCES drink_type (drinkName)
);

CREATE TABLE Allergen_reminders (
    drinkName VARCHAR(255),
    allergen_reminder VARCHAR(255), /*The allergens in the ingredients are used as a separate table*/
    CONSTRAINT fk_Allergen_reminder FOREIGN KEY (drinkName) REFERENCES drink_type (drinkName)
);

CREATE TABLE drink_preservative (
    packaging_design VARCHAR(255),
    preservative_sort VARCHAR(255), /*The preservatives in the ingredients are used as a separate table*/
    CONSTRAINT fk_packaging_preservative FOREIGN KEY (packaging_design) REFERENCES packaging_design (packaging_design_name)
);

CREATE TABLE drink_factory (
    name VARCHAR(255),
    location VARCHAR(255),
    food_production_certificate VARCHAR(255),
    raw_ingredients VARCHAR(255),
    `remaining raw ingredients (per t)` DOUBLE DEFAULT 0,
    CONSTRAINT UQ_factory UNIQUE(name,location,food_production_certificate),
    CONSTRAINT fk_ri1 FOREIGN KEY (raw_ingredients) REFERENCES all_ingredient (ingredient)
);

