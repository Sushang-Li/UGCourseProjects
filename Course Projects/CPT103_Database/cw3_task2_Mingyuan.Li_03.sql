/*
* @description 添加注释
* @author  Mingyuan.Li
* @date  29.11.2022
* @version 0.3
*/
SET FOREIGN_KEY_CHECKS = 0;
drop table if exists packaging_design;
drop table if exists main_material_storage;
drop table if exists material_supplier;
drop table if exists material_supply;
drop table if exists material_order;
SET FOREIGN_KEY_CHECKS = 1;

/*task2 部分*/
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

/*task1 部分*/
CREATE TABLE packaging_design(
    packaging_design_name VARCHAR(255) PRIMARY KEY,
    main_material VARCHAR(100) NOT NULL,
    milliltres INT NOT NULL,
    CONSTRAINT fk_packagingdesign_mainmaterialstorage FOREIGN KEY (main_material) REFERENCES main_material_storage(main_material) /*添加的外键*/
);