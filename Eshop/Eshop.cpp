#include "Eshop.h"

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
	cart.Set_Sum(cart.Get_Sum() + product.Get_Price());
}

void Customer::Remove_From_Cart(Product product) {
	cart.Remove(product);
	cart.Set_Sum(cart.Get_Sum() - product.Get_Price());
}

Product Cart::Get_Item(int id) {
	for (int i = 0; i < products.size(); i++) {
		if (products[i].Get_Id() == id) {
			return products[i];
		}
	}
	return Product();
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

Eshop::Eshop(QWidget* parent) : QMainWindow(parent) {
    ui.setupUi(this);
	Set_headers();
    connect(ui.Button_LoadData, SIGNAL(clicked()), this, SLOT(Set_Shop()));
	connect(ui.List_Shop, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(Double_Clicked_to_Cart()));
	connect(ui.List_Cart, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(Double_Clicked_to_Shop()));
	connect(ui.Slider_Budget, SIGNAL(valueChanged(int)), ui.Line_Budget, SLOT(setValue(int)));
	connect(ui.Line_Budget, SIGNAL(valueChanged(int)), ui.Slider_Budget, SLOT(setValue(int)));
	connect(ui.Line_Search, SIGNAL(textChanged(QString)), this, SLOT(Filter_Table()));
	connect(ui.Button_Purchase, SIGNAL(clicked()), this, SLOT(Save_to_File()));
	connect(ui.Button_Reset_Cart, SIGNAL(clicked()), this, SLOT(Reset_Cart()));
	connect(ui.Button_Reset_All, SIGNAL(clicked()), this, SLOT(Reset_Shop_Data()));

}

Eshop::~Eshop() {}

void Eshop::Set_Shop() {
	Set_Customer();
	Load_File();
}

void Eshop::debug() {
	qDebug() << "--------Customer--------";
	qDebug() << customer.Get_Name().toStdString() << customer.Get_Surname().toStdString() << customer.Get_Money();
	customer.Get_Cart().Print();
	qDebug() << "--------Shop--------";
	Print_Shop();
}

void Eshop::Double_Clicked_to_Cart() {
	int id = ui.List_Shop->item(ui.List_Shop->currentRow(), 0)->text().toInt();
	Product prod = Get_Item(id);
	if (prod.Get_Quantity() > 0 && customer.Get_Money() >= prod.Get_Price()) {
		customer.Add_To_Cart(prod);
		customer.Set_Money(customer.Get_Money() - prod.Get_Price());
		Reduce_Item(id);
		ui.Line_Cart_Money->setText(QString::number(customer.Get_Cart().Get_Sum()));
		ui.Line_Cust_Money->setText(QString::number(customer.Get_Money()));
	}
	else {
		QMessageBox::information(this, "Error", "Not enough money or product is out of stock");
	}
	Load_Cart_to_List();
}

void Eshop::Double_Clicked_to_Shop() {
	int id = ui.List_Cart->item(ui.List_Cart->currentRow(), 0)->text().toInt();
	customer.Remove_From_Cart(customer.Get_Cart().Get_Item(id));
	customer.Set_Money(customer.Get_Money() + Get_Item(id).Get_Price());
	Add_Item(id);
	ui.Line_Cart_Money->setText(QString::number(customer.Get_Cart().Get_Sum()));
	ui.Line_Cust_Money->setText(QString::number(customer.Get_Money()));
	Load_Cart_to_List();
	
}

void Eshop::Reset_Cart() {
	int cartSize = customer.Get_Cart().Get_Size();
	for (int i = cartSize - 1; i >= 0; i--) {
		int quantity = customer.Get_Cart().Get_Index(i).Get_Quantity();
		int productId = customer.Get_Cart().Get_Index(i).Get_Id();
		float productPrice = customer.Get_Cart().Get_Index(i).Get_Price();

		for (int j = quantity - 1; j >= 0; j--) {
			Add_Item(productId);
			customer.Remove_From_Cart(customer.Get_Cart().Get_Index(i));
			customer.Set_Money(customer.Get_Money() + productPrice);
		}
	}
	ui.Line_Cart_Money->setText(QString::number(customer.Get_Cart().Get_Sum()));
	ui.Line_Cust_Money->setText(QString::number(customer.Get_Money()));
	Load_Cart_to_List();
	Load_Shop_to_List();
}

void Eshop::Reset_Shop_Data() {
	Shop = QVector<Product>();
	customer = Customer("", "", 0, Cart());
	ui.Line_Cart_Money->setText(QString::number(customer.Get_Cart().Get_Sum()));
	ui.Line_Cust_Money->setText(QString::number(customer.Get_Money()));
	Set_headers();
	Load_Cart_to_List();
	Load_Shop_to_List();
}

void Eshop::Filter_Table() {
	int switcher = 1;
	if (ui.Box_Search->currentText() == "Distributor") {
		switcher = 2;
	}
	QString name = ui.Line_Search->text();
	for (int row = 0; row < ui.List_Shop->rowCount(); ++row) {
		bool shouldShow = ui.List_Shop->item(row, switcher)->text().contains(name, Qt::CaseInsensitive);
		ui.List_Shop->setRowHidden(row, !shouldShow);
	}
}

void Eshop::Save_to_File() {
	QString filePath = QFileDialog::getSaveFileName(nullptr, "Save File", QFileInfo(QCoreApplication::applicationDirPath()).dir().filePath("Data/listok.txt"));
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "File not opened";
		return;
	}
	QTextStream out(&file);
	out << "<--------------------Customer-------------------->\n";
	out << "Customer:\t" << customer.Get_Name() << " " << customer.Get_Surname() << "\t\tBudget: " << customer.Get_Money() + customer.Get_Cart().Get_Sum() <<"\n";
	out << "<----------------------Cart---------------------->\n";
	out << "ID Name Distributor Quantity Price\n";
	for (int i = 0; i < customer.Get_Cart().Get_Size(); i++) {
		Product product = customer.Get_Cart().Get_Index(i);
		out << product.Get_Name() << "\t" << product.Get_Distributor() << "\tQnt: " << product.Get_Quantity() << "\tPrice: " << product.Get_Price() << "\tTotal: " << product.Get_Price() * product.Get_Quantity() << "\n";
	}
	out << "<------------------------------------------------>\n";
	out << ".....Total.....: \t\t\t\t\t\t" << customer.Get_Cart().Get_Sum() << "\n";
	out << "...Cash left...: \t\t\t\t\t\t" << customer.Get_Money() << "\n";
	file.close();
	Reset_Shop_Data();
}

void Eshop::Set_Customer() {
	QString name = ui.Line_Name->text();
	QString surname = ui.Line_Surname->text();
	float budget = ui.Line_Budget->text().toFloat();
	customer = Customer(name, surname, budget, Cart());
	ui.Line_Cust_Money->setText(QString::number(customer.Get_Money()));
}

void Eshop::Print_Shop() {
	for (int i = 0; i < Shop.size(); i++) {
		qDebug() << Shop[i].Get_Id() << Shop[i].Get_Name().toStdString() << Shop[i].Get_Distributor().toStdString() << Shop[i].Get_Price() << Shop[i].Get_Quantity();
	}
}

void Eshop::Load_File() {
	if (customer.Get_Name() == "" || customer.Get_Surname() == "" || customer.Get_Money() == 0) {
		QMessageBox::information(this, "Error", "Please fill in all the customer data");
		return;
	};
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
	std::sort(Shop.begin(), Shop.end(), [](Product a, Product b) { return a.Get_Id() < b.Get_Id(); });
	Load_Shop_to_List();
	ui.Button_LoadData->setEnabled(false);
	file.close();
}

void Eshop::Load_Shop_to_List() {
	ui.List_Shop->setRowCount(Shop.size());

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

	for (int i = 0; i < customer.Get_Cart().Get_Size(); i++) {
		ui.List_Cart->setItem(i, 0, new QTableWidgetItem(QString::number(customer.Get_Cart().Get_Index(i).Get_Id())));
		ui.List_Cart->setItem(i, 1, new QTableWidgetItem(customer.Get_Cart().Get_Index(i).Get_Name()));
		ui.List_Cart->setItem(i, 2, new QTableWidgetItem(customer.Get_Cart().Get_Index(i).Get_Distributor()));
		ui.List_Cart->setItem(i, 3, new QTableWidgetItem(QString::number(customer.Get_Cart().Get_Index(i).Get_Price())));
		ui.List_Cart->setItem(i, 4, new QTableWidgetItem(QString::number(customer.Get_Cart().Get_Index(i).Get_Quantity())));
	}
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

Product Eshop::Get_Item(int id) {
	for (int i = 0; i < Shop.size(); i++) {
		if (Shop[i].Get_Id() == id) {
			return Shop[i];
		}
	}
	return Product();
}

void Eshop::Set_headers() {
	ui.Line_Name->setText("");
	ui.Line_Name->setPlaceholderText("Name");
	ui.Line_Surname->setText("");
	ui.Line_Surname->setPlaceholderText("Surname");
	ui.Line_Budget->setValue(0);
	ui.Line_Cart_Money->setPlaceholderText("0");
	ui.Line_Cart_Money->setReadOnly(true);
	ui.Line_Cust_Money->setReadOnly(true);
	ui.Button_LoadData->setEnabled(true);

	ui.Line_Cust_Money->setText(QString::number(0));
	ui.Line_Cart_Money->setText(QString::number(0));

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
}
