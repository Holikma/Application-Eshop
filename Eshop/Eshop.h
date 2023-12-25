#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Eshop.h"


class Product {
	private:
		QString name;
		float price;
		int quantity;

	public:
		Product(QString name, float price, int quantity);
};

class Cart {
	private:
		int size;
		QList<Product> products;

	public:
		Cart();
};

class Customer {
	private:
		QString Name;
		QString Surname;
		Cart cart;

	public:
		Customer() {};
		Customer(QString name, QString Surname, Cart cart);
};

class Eshop : public QMainWindow{
	Q_OBJECT
	private:
		Customer customer;
		Ui::EshopClass ui;

	private slots:
		void LoadProducts();

    public:
        Eshop(QWidget *parent = nullptr);
        ~Eshop();
};

