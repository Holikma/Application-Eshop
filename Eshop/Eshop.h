#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Eshop.h"
#include <QFileDialog>
#include <QDir>
#include <QVector>
#include <QFile>
#include <QDebug>
#include <QMessageBox>

class Product {
	private:
		int id;
		QString name;
		QString distributor;
		int quantity;
		float price;

	public:
		Product() : id(), name(), distributor(), price(), quantity() {};
		Product(int id, QString name, QString distributor, int quantity, float price);
		int Get_Id() { return id; };
		int Get_Quantity() { return quantity; };
		void Set_Quantity(int quantity) { this->quantity = quantity; };
		float Get_Price() { return price; };
		QString Get_Name() { return name; };
		QString Get_Distributor() { return distributor; };
};

class Cart {
	private:
		QVector<Product> products;

	public:
		Cart() : products() {};
		int Get_Size() { return products.size(); };
		void Add(Product product);
		void Remove(Product product);
		void Print();
		Product Get_Item(int id);
		Product Get_Index(int index) { return products[index]; };
};

class Customer {
	private:
		QString Name;
		QString Surname;
		float money;
		Cart cart;

	public:
		Customer() : Name(), Surname(), money(), cart() {};
		Customer(QString name, QString Surname, float money, Cart cart);
		void Add_To_Cart(Product product);
		void Remove_From_Cart(Product product);
		void Set_Money(float money) { this->money = money; };
		float Get_Money() { return money; };
		Cart Get_Cart() { return cart; };
		QString Get_Name() { return Name; };
		QString Get_Surname() { return Surname; };
};

class Eshop : public QMainWindow{
	Q_OBJECT
	private:
		Customer customer;
		QVector<Product> Shop;
		Ui::EshopClass ui;

	private slots:
		void Set_Shop();
		void debug();
		void Double_Clicked_to_Cart();
		void Double_Clicked_to_Shop();
		void Filter_Table();

    public:
        Eshop(QWidget *parent = nullptr);
        ~Eshop();
		void Set_Customer();
		void Print_Shop();
		void Load_File();
		void Load_Shop_to_List();
		void Load_Cart_to_List();
		void Reduce_Item(int id);
		void Add_Item(int id);
		Product Get_Item(int id);
};

