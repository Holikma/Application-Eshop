#include "Eshop.h"


Eshop::Eshop(QWidget* parent) : QMainWindow(parent) {
    ui.setupUi(this);
	ui.Line_Name->setPlaceholderText("Name");
	ui.Line_Surname->setPlaceholderText("Surname");

    connect(ui.Button_LoadData, SIGNAL(clicked()), this, SLOT(Set_Shop()));
	connect(ui.Button_Debug, SIGNAL(clicked()), this, SLOT(debug()));
	connect(ui.List_Shop, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(Double_Clicked_to_Cart()));
	connect(ui.List_Cart, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(Double_Clicked_to_Shop()));
	connect(ui.Slider_Budget, SIGNAL(valueChanged(int)), ui.Line_Budget, SLOT(setValue(int)));
	connect(ui.Line_Budget, SIGNAL(valueChanged(int)), ui.Slider_Budget, SLOT(setValue(int)));
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
	Set_Budget();
	Load_File();
	Load_Shop_to_List();
}

void Eshop::Set_Budget() {	
	customer.Set_Money(ui.Line_Budget->text().toFloat());
}

void Eshop::Reduce_Item(int id) {
	for (int i = 0; i < Shop.size(); i++) {
		if (Shop[i].Get_Id() == id) {
			Shop[i].Set_Quantity(Shop[i].Get_Quantity() - 1);
		}
	}
	Load_Shop_to_List();
}

void Eshop::Add_Item(int id) {
	for (int i = 0; i < Shop.size(); i++) {
		if (Shop[i].Get_Id() == id) {
			Shop[i].Set_Quantity(Shop[i].Get_Quantity() + 1);
		}
	}
	Load_Shop_to_List();
}

void Eshop::Double_Clicked_to_Cart() {
	int id = ui.List_Shop->item(ui.List_Shop->currentRow(), 0)->text().toInt();
	Product prod = Get_Item(id);
	if (prod.Get_Quantity() > 0 && customer.Get_Money() >= prod.Get_Price()) {
		customer.Add_To_Cart(prod);
		customer.Set_Money(customer.Get_Money() - prod.Get_Price());
		Reduce_Item(id);
	}
	Load_Cart_to_List();
}

void Eshop::Double_Clicked_to_Shop() {
	int id = ui.List_Cart->item(ui.List_Cart->currentRow(), 0)->text().toInt();
	customer.Remove_From_Cart(customer.Get_Cart().Get_Item(id));
	customer.Set_Money(customer.Get_Money() + Get_Item(id).Get_Price());
	Add_Item(id);
	Load_Cart_to_List();
	
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

void Eshop::Load_Cart_to_List() {
	ui.List_Cart->setRowCount(customer.Get_Cart().Get_Size());
	ui.List_Cart->setColumnCount(5);

	ui.List_Cart->setHorizontalHeaderItem(0, new QTableWidgetItem("ID"));
	ui.List_Cart->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
	ui.List_Cart->setHorizontalHeaderItem(2, new QTableWidgetItem("Distr."));
	ui.List_Cart->setHorizontalHeaderItem(3, new QTableWidgetItem("$$$"));
	ui.List_Cart->setHorizontalHeaderItem(4, new QTableWidgetItem("Qnt."));

	ui.List_Cart->verticalHeader()->setVisible(false);
	ui.List_Cart->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.List_Cart->setSelectionBehavior(QAbstractItemView::SelectRows);

	ui.List_Cart->setColumnWidth(0, 30);
	ui.List_Cart->setColumnWidth(1, 80);
	ui.List_Cart->setColumnWidth(2, 80);
	ui.List_Cart->setColumnWidth(3, 35);
	ui.List_Cart->setColumnWidth(4, 35);

	for (int i = 0; i < customer.Get_Cart().Get_Size(); i++) {
		ui.List_Cart->setItem(i, 0, new QTableWidgetItem(QString::number(customer.Get_Cart().Get_Index(i).Get_Id())));
		ui.List_Cart->setItem(i, 1, new QTableWidgetItem(customer.Get_Cart().Get_Index(i).Get_Name()));
		ui.List_Cart->setItem(i, 2, new QTableWidgetItem(customer.Get_Cart().Get_Index(i).Get_Distributor()));
		ui.List_Cart->setItem(i, 3, new QTableWidgetItem(QString::number(customer.Get_Cart().Get_Index(i).Get_Price())));
		ui.List_Cart->setItem(i, 4, new QTableWidgetItem(QString::number(customer.Get_Cart().Get_Index(i).Get_Quantity())));
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
	bool found = false;
	for (int i = 0; i < products.size(); i++) {
		if (products[i].Get_Id() == product.Get_Id()) {
			products[i].Set_Quantity(products[i].Get_Quantity() + 1);
			found = true;
			break;
		}
	}
	if (!found) {
		products.append(product);
		products[products.size()-1].Set_Quantity(1);
	}
	std::sort(products.begin(), products.end(), []( Product a, Product b) { return a.Get_Id() < b.Get_Id(); });
}

void Customer::Remove_From_Cart(Product product) {
	cart.Remove(product);
}

void Cart::Remove(Product product) {
	for (int i = 0; i < products.size(); i++) {
		if (products[i].Get_Id() == product.Get_Id()) {
			products[i].Set_Quantity(products[i].Get_Quantity() - 1);
			if (products[i].Get_Quantity() == 0) {
				products.remove(i);
			}
			break;
		}
	}
	
}

void Cart::Print() {
	for (int i = 0; i < products.size(); i++) {
		qDebug() << products[i].Get_Id() << products[i].Get_Name().toStdString() << products[i].Get_Distributor().toStdString() << products[i].Get_Price() << products[i].Get_Quantity();
	}
}

Product Cart::Get_Item(int id) {
	for (int i = 0; i < products.size(); i++) {
		if (products[i].Get_Id() == id) {
			return products[i];
		}
	}
	return Product();
}
