#include <iostream>
#include <cmath>

enum colorsId {none, black, blue, green, cyan, red, purple, yellow, white};

class Polygon {
protected:
    std::pair<double, double> basePoint {0, 0};
    colorsId color = none;

public:
    virtual void area () =0;
    virtual void circumrectangle ()=0;


    void print () {
        std::cout << typeid(*this).name() << std::endl;
        area();
        circumrectangle();
        std::cout << std::endl;
    }

    Polygon (double a, double b) : basePoint(a, b) {}
};

class Rectangle: public Polygon {
private:
    double width = 1, height = 1;

public:
    void area () {
        std::cout << "Area: " << width * height << std::endl;
    }

    void circumrectangle () {
        std::cout << "Circumrectangle (Width x Height): " << width << " x " << height << std::endl;
    }

    Rectangle (double a, double b, double w, double h) : Polygon(a,b), width(fabs(w)), height(fabs(h)) {

    }
};

class Square: public Polygon {
private:
    double side = 1;

public:
    void area () {
        std::cout << "Area: " << side * side << std::endl;
    }

    void circumrectangle () {
        std::cout << "Circumrectangle (Width x Height): " << side << " x " << side << std::endl;
    }

    Square (double a, double b, double s) : Polygon(a,b), side(fabs(s)) {}
};

class Triangle: public Polygon {
private:
    double side = 1;

public:
    void area () {
        std::cout << "Area: " << side * side * std::sqrt(3) / 4 << std::endl;
    }

    void circumrectangle () {
        std::cout << "Circumrectangle (Width x Height): " << side << " x " << side * std::sqrt(3) / 2 << std::endl;
    }

    Triangle (double a, double b, double s) : Polygon(a,b), side(fabs(s)) {}
};

class Circle: public Polygon {
private:
    double radius = 1;

public:
    void area () {
        std::cout << "Area: " << atan(1) * 4 * radius << std::endl;
    }

    void circumrectangle () {
        std::cout << "Circumrectangle (Width x Height): " << radius * 2 << " x " << radius * 2 << std::endl;
    }

    Circle (double a, double b, double r) : Polygon(a,b), radius(fabs(r)) {}
};

template<typename Type>
Type inputSomething (const std::string & previewText) {
    Type result;
    while (true) {
        std::cin.rdbuf()->pubsync();
        std::cin.clear();
        std::cout << previewText;
        std::cin >> result;
        if (!std::cin.fail()) {
            break;
        }
    }
    return result;
}

template<typename Type>
void inputPoligon (const std::string & previewText) {
    std::cout << previewText;
    double a, b, s;
    std::cin >> a >> b >> s;
    Type shape(a, b, s);
    shape.print();
}

void inputPoligon (const std::string & previewText) {
    std::cout << previewText;
    double a, b, w, h;
    std::cin >> a >> b >> w >> h;
    Rectangle shape(a, b, w, h);
    shape.print();
}

int main() {
    while (true) {
        auto command = inputSomething<std::string> ("Input command: ");

        if (command == "rectangle") {
            inputPoligon("Input base point (a,b) and Width and Height: ");
        }
        else if (command == "square") {
            inputPoligon<Square>("Input base point (a,b) and side: ");
        }
        else if (command == "triangle"){
            inputPoligon<Triangle>("Input base point (a,b) and side: ");
        }
        else if (command == "circle") {
            inputPoligon<Circle>("Input base point (a,b) and radius: ");
        }
        else if (command == "exit"){
            break;
        }
        else {
            std::cout << "Unknown command" << std::endl;
        }
    }
    return 0;
}