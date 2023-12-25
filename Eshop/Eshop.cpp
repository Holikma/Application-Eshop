#include "Eshop.h"


Eshop::Eshop(QWidget* parent) : QMainWindow(parent) {
    ui.setupUi(this);
	ui.Line_Name->setPlaceholderText("Name");
	ui.Line_Surname->setPlaceholderText("Surname");
    connect(ui.Button_LoadData, SIGNAL(clicked()), this, SLOT(LoadProducts()));

}

Eshop::~Eshop() {}

void Eshop::LoadProducts() {

}


Product::Product(QString name, float price, int quantity) {
	this->name = name;
    this->price = price;
    this->quantity = quantity;
}

Customer::Customer(QString Name, QString Surname, Cart cart) {
	this->Name = Name;
	this->Surname = Surname;
	this->cart = cart;
}

Cart::Cart() {
	products = {};
	size = 0;
}