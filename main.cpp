#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
using namespace std;

class BigInt {
    string number;    // Stores the number as a string
    bool isNegative;  // True if number is negative

    // Remove unnecessary leading zeros from the number string
    void removeLeadingZeros() {
        size_t i = 0;
        while (i < number.size() - 1 && number[i] == '0')
            i++;

        number = number.substr(i);
        if (number == "0")
            isNegative = false;
    }

    // Compare absolute values of two BigInts (ignore signs)
    // Returns: 1 if |this| > |other|, 0 if equal, -1 if |this| < |other|
    int compareMagnitude(const BigInt& other) const {
        if (number.size() != other.number.size())
            return (number.size() > other.number.size()) ? 1 : -1;

        if (number == other.number)
            return 0;

        return (number > other.number) ? 1 : -1;
    }

    static string addMagnitudes(const string& a, const string& b){
        string result;
        int carry = 0, i = (int)a.size() - 1, j = (int)b.size() - 1;
        while (i >= 0 || j >= 0 || carry)
        {
            int sum = carry;
            if (i >= 0) sum += a[i--]-'0';
            if (j >= 0) sum += b[j--]-'0';
            carry = sum / 10;
            result += (char)('0' + sum % 10);
        }
        for (int l = 0, r = (int)result.size() - 1; l < r; l++, r--)
            swap(result[l], result[r]);

        return result.empty() ? "0" : result;
    }

    static string subtractMagnitudes(const string& a, const string& b) {
        string result;
        int borrow = 0, i = (int)a.size() - 1, j = (int)b.size() - 1;
        while (i >= 0)
        {
            int diff = (a[i--]-'0') - borrow;
            if (j >= 0) diff -= (b[j--]-'0');
            if (diff < 0) { diff += 10; borrow = 1; } else borrow = 0;
            result += (char)('0' + diff);
        }
        for (int l = 0, r = (int)result.size() - 1; l < r; l++, r--)
            swap(result[l], result[r]);

        return result.empty() ? "0" : result;
    }

    static string mulMagByDigit(const string& a, int d) {
        if (d == 0)
            return "0";

        string result;
        int carry = 0;
        for (int i = (int)a.size() - 1; i >= 0; i--)
        {
            int prod = (a[i] - '0') * d + carry;
            carry = prod / 10;
            result += (char)('0' + prod % 10);
        }
        while (carry)
        {
            result += (char)('0' + carry % 10);
            carry /= 10;
        }
        for (int l = 0, r = (int)result.size() - 1; l < r; l++, r--)
            swap(result[l], result[r]);

        return result;
    }

    static bool magLE(const string& a, const string& b){
        if (a.size() != b.size())
            return a.size() < b.size();

        return a <= b;
    }

public:
    // Default constructor - initialize to zero
    BigInt() {
        number = "0";
        isNegative = false;
    }

    // Constructor from 64-bit integer
    BigInt(int64_t value) {
        if (value < 0) {
            isNegative = true;
            number = std::to_string(-value);
        }
        else {
            isNegative = false;
            number = std::to_string(value);
        }
    }

    // Constructor from string representation
    BigInt(const string& str) {
        // TODO: Implement this constructor
        // Check if first character is '-'
        // Set isNegative accordingly
        // Extract digits (skip sign if present)
        // Store in 'number'
        // Call removeLeadingZeros()

        string numString=str;
        if (!numString.empty() && numString[0]=='-'){
            isNegative=true;
            numString.erase(0,1);
        }
        else{
            isNegative=false;
        }
        number=numString;
        removeLeadingZeros();

    }

    // Copy constructor
    BigInt(const BigInt& other) {
        this->number = other.number;
        this->isNegative = other.isNegative;
    }

    // Destructor
    ~BigInt() {
        // Empty - no dynamic memory to clean up
    }

    // Assignment operator
    BigInt& operator=(const BigInt& other) {
        // TODO: Implement this operator
        return *this;
    }

    // Unary negation operator (-x)
    BigInt operator-() const {
        BigInt result;
        // TODO: Implement negation logic
        return result;
    }

    // Unary plus operator (+x)
    BigInt operator+() const {
        BigInt result;
        // TODO: Implement this operator
        return result;
    }

    // Addition assignment operator (x += y)
    BigInt& operator+=(const BigInt& other) {
        if (isNegative == other.isNegative)
        {
            number = addMagnitudes(number, other.number);
        }
        else
        {
            int cmp = compareMagnitude(other);
            if (cmp == 0)
            { 
                number = "0"; isNegative = false;
            }
            else if (cmp > 0)
            {
                number = subtractMagnitudes(number, other.number);
            }
            else
            { 
                number = subtractMagnitudes(other.number, number); 
                isNegative = other.isNegative;
            }
        }
        removeLeadingZeros();
        return *this;
    }

    // Subtraction assignment operator (x -= y)
    BigInt& operator-=(const BigInt& other) {
        // TODO: Implement this operator
        return *this;
    }

    // Multiplication assignment operator (x *= y)
    BigInt& operator*=(const BigInt& other) {
        // TODO: Implement this operator
        if (number == "0" || other.number == "0") {
            number = "0";           
            isNegative = false;     
            return *this;          
        }
        bool resultNeg= (isNegative != other.isNegative);
        vector<int> digits1, digits2;
        for (int i = number.length() - 1; i >= 0; i--) {
            digits1.push_back(number[i] - '0');  // '5' becomes 5
         }
        for (int i = other.number.length() - 1; i >= 0; i--) {
            digits2.push_back(other.number[i] - '0');
        }
        int len1=digits1.size();
        int len2=digits2.size();
        vector<int> result(len1 + len2, 0);
        for (int i=0;i<len1;i++){
            for (int j=0;j<len2;j++){
                result[i + j] += digits1[i] * digits2[j];
            }
        }
            int carry = 0;
        for (size_t i = 0; i < result.size(); i++) {
            result[i] += carry;
            carry = result[i] / 10;
            result[i] %= 10;
        }
        
       
        while (result.size() > 1 && result.back() == 0) {
            result.pop_back();
        }
        
        
        string newNumber;
        for (int i = result.size() - 1; i >= 0; i--) {
            newNumber += (result[i] + '0');
        }
    
    
        number = newNumber;
        isNegative = resultNeg;
        
        return *this;
    }
      

    // Division assignment operator (x /= y)
    BigInt& operator/=(const BigInt& other) {
        // TODO: Implement this operator
        return *this;
    }

    // Modulus assignment operator (x %= y)
    BigInt& operator%=(const BigInt& other) {
        // TODO: Implement this operator
        return *this;
    }

    // Pre-increment operator (++x)
    BigInt& operator++() {
        // TODO: Implement this operator
        return *this;
    }

    // Post-increment operator (x++)
    BigInt operator++(int) {
        BigInt temp;
        // TODO: Implement this operator
        return temp;
    }

    // Pre-decrement operator (--x)
    BigInt& operator--() {
        // TODO: Implement this operator
        return *this;
    }

    // Post-decrement operator (x--)
    BigInt operator--(int) {
        BigInt temp;
        // TODO: Implement this operator
        return temp;
    }

    // Convert BigInt to string representation
    string toString() const {
        if (isNegative && number != "0"){
            return "-" + number;
        }
        return number;
    }

    // Output stream operator (for printing)
    friend ostream& operator<<(ostream& os, const BigInt& num) {
        if (num.isNegative && num.number != "0") {
            os << '-';
        }
        os << num.number;
        return os;
    }

    // Input stream operator (for reading from input)
    friend istream& operator>>(istream& is, BigInt& num) {
           string s;
           is >> s;
           BigInt temp(s);
           num = temp;
        return is;
    }

    // Friend declarations for comparison operators
    friend bool operator==(const BigInt& lhs, const BigInt& rhs);
    friend bool operator<(const BigInt& lhs, const BigInt& rhs);
};

// Binary addition operator (x + y)
BigInt operator+(BigInt lhs, const BigInt& rhs) {
    lhs += rhs;
    return lhs;
}

// Binary subtraction operator (x - y)
BigInt operator-(BigInt lhs, const BigInt& rhs) {
    BigInt result;
    // TODO: Implement this operator
    return result;
}

// Binary multiplication operator (x * y)
BigInt operator*(BigInt lhs, const BigInt& rhs) {
    
    // TODO: Implement this operator
    return lhs*=rhs;
}

// Binary division operator (x / y)
BigInt operator/(BigInt lhs, const BigInt& rhs) {
    BigInt result;
    // TODO: Implement this operator
    return result;
}

// Binary modulus operator (x % y)
BigInt operator%(BigInt lhs, const BigInt& rhs) {
    BigInt result;
    // TODO: Implement this operator
    return result;
}

// Equality comparison operator (x == y)
bool operator==(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    return false;
}

// Inequality comparison operator (x != y)
bool operator!=(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    return false;
}

// Less-than comparison operator (x < y)
bool operator<(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    return false;
}

// Less-than-or-equal comparison operator (x <= y)
bool operator<=(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    return false;
}

// Greater-than comparison operator (x > y)
bool operator>(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    return false;
}

// Greater-than-or-equal comparison operator (x >= y)
bool operator>=(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    return false;
}

int main() {
    cout << "=== BigInt Class Test Program ===" << endl << endl;
    cout << "NOTE: All functions are currently empty." << endl;
    cout << "Your task is to implement ALL the functions above." << endl;
    cout << "The tests below will work once you implement them correctly." << endl << endl;

    // Test 1: Constructors and basic output
    cout << "1. Constructors and output:" << endl;
    BigInt a(12345);              // Should create BigInt from integer
    BigInt b("-67890");           // Should create BigInt from string
    BigInt c("0");                // Should handle zero correctly
    BigInt d = a;                 // Should use copy constructor
    cout << "a (from int): " << a << endl;        // Should print "12345"
    cout << "b (from string): " << b << endl;     // Should print "-67890"
    cout << "c (zero): " << c << endl;            // Should print "0"
    cout << "d (copy of a): " << d << endl << endl; // Should print "12345"D

    
    // Test 2: Arithmetic operations
    //cout << "2. Arithmetic operations:" << endl;
    //cout << "a + b = " << a + b << endl;          // Should calculate 12345 + (-67890)
    //cout << "a - b = " << a - b << endl;          // Should calculate 12345 - (-67890)
    cout << "a * b = " << a * b << endl;          // Should calculate 12345 * (-67890)
    //cout << "b / a = " << b / a << endl;          // Should calculate (-67890) / 12345
    //cout << "a % 100 = " << a % BigInt(100) << endl << endl; // Should calculate 12345 % 100
    /*
    // Test 3: Relational operators
    cout << "3. Relational operators:" << endl;
    cout << "a == d: " << (a == d) << endl;       // Should be true (12345 == 12345)
    cout << "a != b: " << (a != b) << endl;       // Should be true (12345 != -67890)
    cout << "a < b: " << (a < b) << endl;         // Should be false (12345 < -67890)
    cout << "a > b: " << (a > b) << endl;         // Should be true (12345 > -67890)
    cout << "c == 0: " << (c == BigInt(0)) << endl << endl; // Should be true (0 == 0)

    // Test 4: Unary operators and increments
    cout << "4. Unary operators and increments:" << endl;
    cout << "-a: " << -a << endl;                 // Should print "-12345"
    cout << "++a: " << ++a << endl;               // Should increment and print "12346"
    cout << "a--: " << a-- << endl;               // Should print "12346" then decrement
    cout << "a after decrement: " << a << endl << endl; // Should print "12345"

    // Test 5: Large number operations
    cout << "5. Large number operations:" << endl;
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    cout << "Very large addition: " << num1 + num2 << endl;
    cout << "Very large multiplication: " << num1 * num2 << endl << endl;

    // Test 6: Edge cases and error handling
    cout << "6. Edge cases:" << endl;
    BigInt zero(0);
    BigInt one(1);
    try {
        BigInt result = one / zero;               // Should throw division by zero error
        cout << "Division by zero succeeded (unexpected)" << endl;
    } catch (const runtime_error& e) {
        cout << "Division by zero correctly threw error: " << e.what() << endl;
    }
    cout << "Multiplication by zero: " << one * zero << endl;        // Should be "0"
    cout << "Negative multiplication: " << BigInt(-5) * BigInt(3) << endl;  // Should be "-15"
    cout << "Negative division: " << BigInt(-10) / BigInt(3) << endl;       // Should be "-3"
    cout << "Negative modulus: " << BigInt(-10) % BigInt(3) << endl;        // Should be "-1"
    */
    return 0;
}
