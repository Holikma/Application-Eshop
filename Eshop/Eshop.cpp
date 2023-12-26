#include "Eshop.h"


Eshop::Eshop(QWidget* parent) : QMainWindow(parent) {
    ui.setupUi(this);
	ui.Line_Name->setPlaceholderText("Name");
	ui.Line_Surname->setPlaceholderText("Surname");
    connect(ui.Button_LoadData, SIGNAL(clicked()), this, SLOT(Set_Shop()));
	connect(ui.Button_Debug, SIGNAL(clicked()), this, SLOT(debug()));
	connect(ui.List_Shop, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(Double_Clicked()));
}

Eshop::~Eshop() {}

void Eshop::Set_Customer() {
	customer = Customer(ui.Line_Name->text(), ui.Line_Surname->text(), ui.Line_Budget->text().toFloat(), Cart());
}

void Eshop::Load_File() {
	QString filePath = QFileDialog::getOpenFileName(nullptr, "Open File", QFileInfo(QCoreApplication::applicationDirPath()).dir().filePath("Data/produkty.txt"));
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "File not opened";
		return;
	}
	QTextStream in(&file);
	QString line = in.readLine();
	line = in.readLine();
	while (!line.isNull()) {
		QStringList list = line.split(" ");
		Shop.append(Product(list[0].toInt(), list[1], list[2], list[3].toInt(), list[4].toFloat()));
		line = in.readLine();
	}
	Print_Shop();
	file.close();
}

void Eshop::Set_Shop() {
	Set_Customer();
	Load_File();
	Load_Shop_to_List();
}

void Eshop::Double_Clicked() {
	int id = ui.List_Shop->item(ui.List_Shop->currentRow(), 0)->text().toInt();
	Product product = Get_Item(id);
	if (product.Get_Quantity() > 0) {
		if (customer.Get_Money() >= product.Get_Price()) {
			customer.Add_To_Cart(product);
			customer.Set_Money(customer.Get_Money() - product.Get_Price());
			product.Set_Quantity(id);
			Load_Shop_to_List();
		}
		else {
			QMessageBox::information(this, "Error", "Not enough money");
		}
	}
	else {
		QMessageBox::information(this, "Error", "Not enough products");
	}
}

void Eshop::Load_Shop_to_List() {
	ui.List_Shop->setRowCount(Shop.size());
	ui.List_Shop->setColumnCount(5);
	ui.List_Shop->setHorizontalHeaderItem(0, new QTableWidgetItem("ID"));
	ui.List_Shop->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
	ui.List_Shop->setHorizontalHeaderItem(2, new QTableWidgetItem("Distr."));
	ui.List_Shop->setHorizontalHeaderItem(3, new QTableWidgetItem("$$$"));
	ui.List_Shop->setHorizontalHeaderItem(4, new QTableWidgetItem("Qnt."));

	ui.List_Shop->verticalHeader()->setVisible(false);
	ui.List_Shop->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.List_Shop->setSelectionBehavior(QAbstractItemView::SelectRows);


	ui.List_Shop->setColumnWidth(0, 30);
	ui.List_Shop->setColumnWidth(1, 80);
	ui.List_Shop->setColumnWidth(2, 80);
	ui.List_Shop->setColumnWidth(3, 35);
	ui.List_Shop->setColumnWidth(4, 35);

	for (int i = 0; i < Shop.size(); i++) {
		ui.List_Shop->setItem(i, 0, new QTableWidgetItem(QString::number(Shop[i].Get_Id())));
		ui.List_Shop->setItem(i, 1, new QTableWidgetItem(Shop[i].Get_Name()));
		ui.List_Shop->setItem(i, 2, new QTableWidgetItem(Shop[i].Get_Distributor()));
		ui.List_Shop->setItem(i, 3, new QTableWidgetItem(QString::number(Shop[i].Get_Price())));
		ui.List_Shop->setItem(i, 4, new QTableWidgetItem(QString::number(Shop[i].Get_Quantity())));
	}
}

void Eshop::Print_Shop() {
	for (int i = 0; i < Shop.size(); i++) {
		qDebug() << Shop[i].Get_Id() << Shop[i].Get_Name().toStdString() << Shop[i].Get_Distributor().toStdString() << Shop[i].Get_Price() << Shop[i].Get_Quantity();
	}
}

void Eshop::debug() {
	qDebug() << customer.Get_Name().toStdString() << customer.Get_Surname().toStdString() << customer.Get_Money();
	customer.Get_Cart().Print();
}



Product Eshop::Get_Item(int id) {
	for (int i = 0; i < Shop.size(); i++) {
		if (Shop[i].Get_Id() == id) {
			return Shop[i];
		}
	}
	return Product();
}

Product::Product(int id, QString name, QString distributor, int quantity, float price) {
	this->id = id;
	this->name = name;
	this->distributor = distributor;
    this->quantity = quantity;
    this->price = price;
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
		qDebug() << products[i].Get_Id() << products[i].Get_Name().toStdString() << products[i].Get_Distributor().toStdString() << products[i].Get_Price() << products[i].Get_Quantity();
	}
}

