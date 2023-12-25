#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Eshop.h"
#include <QFileDialog>
#include <QDir>
#include <QVector>


class Product {
	private:
		int id;
		QString name;
		QString distributor;
		float price;
		int quantity;

	public:
		Product() : id(), name(), distributor(), price(), quantity() {};
		Product(int id, QString name, QString distributor, float price, int quantity);
		int GetId() { return id; };
		QString GetName() { return name; };
		QString GetDistributor() { return distributor; };
		float GetPrice() { return price; };
		int GetQuantity() { return quantity; };
		
};

class Cart {
	private:
		QVector<Product> products;

	public:
		Cart() : products() {};
		void Add(Product product);
		void Print();
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
		Cart Get_Cart() { return cart; };
};

class Eshop : public QMainWindow{
	Q_OBJECT
	private:
		Customer customer;
		QVector<Product> Shop;
		Ui::EshopClass ui;

	private slots:
		void Set_Shop();

    public:
        Eshop(QWidget *parent = nullptr);
        ~Eshop();

};

