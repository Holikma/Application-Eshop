#include "Eshop.h"


Eshop::Eshop(QWidget* parent) : QMainWindow(parent) {
    ui.setupUi(this);
	ui.Line_Name->setPlaceholderText("Name");
	ui.Line_Surname->setPlaceholderText("Surname");
    connect(ui.Button_LoadData, SIGNAL(clicked()), this, SLOT(Set_Shop()));

}

Eshop::~Eshop() {}

void Eshop::Set_Shop() {
	QString filePath = QFileDialog::getOpenFileName(nullptr, "Open File",
		QFileInfo(QCoreApplication::applicationDirPath()).dir().filePath("Data/produkty.txt"));
	QFile file(filePath);
	customer = Customer(ui.Line_Name->text(), ui.Line_Surname->text(), ui.Line_Budget->text().toFloat(), Cart());
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "File not opened";
		return;
	}

	QTextStream in(&file);
	QString line = in.readLine();
	Shop.resize(line.toInt());
	line = in.readLine();
	for (int i = 0; i < Shop.size(); i++) {
		QStringList list = line.split(" ");
		Product product(list[0].toInt(), list[1], list[2], list[3].toFloat(), list[4].toInt());
		Shop[i] = product;
		line = in.readLine();
		qDebug() << Shop[i].GetId();
	}
	
	file.close();
}


Product::Product(int id, QString name, QString distributor, float price, int quantity) {
	this->id = id;
	this->name = name;
	this->distributor = distributor;
    this->price = price;
    this->quantity = quantity;
}

Customer::Customer(QString Name, QString Surname, float money, Cart cart) {
	this->Name = Name;
	this->Surname = Surname;
	this->money = money;
	this->cart = cart;
}

void Customer::Add_To_Cart(Product product) {
	cart.Add(product);
}


void Cart::Add(Product product) {
	products.append(product);
}

void Cart::Print() {
	for (int i = 0; i < products.size(); i++) {
		qDebug() << products[i].GetId();
		qDebug() << products[i].GetName().toStdString();
		qDebug() << products[i].GetDistributor().toStdString();
		qDebug() << products[i].GetPrice();
		qDebug() << products[i].GetQuantity();
	}
}

