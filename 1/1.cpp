#include <iostream>
#include <time.h>
#include <fstream>
using namespace std;
bool simple(long long int a)
{
    int i;
    for (i = 2; i < sqrt(a); i++)
    {
        if (a % i == 0)
            return false;
    }
    return true;
}

long long int pow_mod(long long int a, long long int n, long long int p)
{
    long long int i, k, n2 = 0, res = 1, * N2;
    /*if (!simple(p))
    {
        cout << " p не простое.\n";
        return 0;
    }*/
    for (i = 1, n2 = 1; i * 2 < n; i *= 2, n2++);
    N2 = new long long int[n2 + 1];
    for (i = 0, k = n; i <= n2; i++, k /= 2)
        N2[i] = k % 2;

    for (i = 0; i <= n2; i++)
    {
        if (i > 0)
            k = ((k % p) * (k % p)) % p;
        else
            k = a % p;
        if (N2[i] > 0)
            res = ((res % p) * (k % p)) % p;
    }
    delete[] N2;
    return res;
}

int evk(long long int a, long long int b, long long int& x, long long int& y)
{
    int i;
    long long int obm, q, U[3] = { a, 1, 0 }, V[3] = { b, 0, 1 }, T[3];
    if (a < b)
    {
        obm = a;
        a = b;
        b = obm;
    }
    while (V[0] != 0)
    {
        q = U[0] / V[0];
        for (i = 0; i < 3; i++)
        {
            if (i > 0)
                T[i] = U[i] - q * V[i];
            else
                T[i] = U[i] % V[i];
            U[i] = V[i];
            V[i] = T[i];
        }
    }
    x = U[1];
    y = U[2];
    return U[0];
}

long long int dh()
{
    long long int p, q = 0, g, X[2], Y[2], Z[2];
    while (!simple(q) || (q == 0) || !simple(p))
    {
        q = rand() % (long long int)pow(10, 9);
        p = 2 * q + 1;
    }
    do
    {
        g = 2 + rand() % (p - 2);
    } while (pow_mod(g, q, p) == 1);
    X[0] = 2 + rand() % (p - 2);
    X[1] = 2 + rand() % (p - 2);
    Y[0] = pow_mod(g, X[0], p);
    Y[1] = pow_mod(g, X[1], p);
    Z[0] = pow_mod(Y[1], X[0], p);
    Z[1] = pow_mod(Y[0], X[1], p);
    //cout << " p = " << p << ", Ya = " << Y[0] << ", Yb = " << Y[1] << ", Za = " << Z[0] << " = Zb = " << Z[1] << endl;
    return Z[1];
}

void shmshv(long long int a, long long int p, long long int y)
{
    long long int i, j, m = sqrt(p) + 1, k = m, * Ay, * A;
    if (!simple(p))
    {
        cout << " p не простое.\n";
        return;
    }
    Ay = new long long int[m];
    A = new long long int[k];
    for (i = 0; i < m; i++)
        Ay[i] = (pow_mod(a, i, p) * (y % p)) % p;

    for (i = 0; i < k; i++)
    {
        A[i] = pow_mod(a, m * (i + 1), p);
        for (j = 0; j < m; j++)
        {
            if (A[i] == Ay[j])
            {
                cout << " i = " << i + 1 << ", j = " << j << ", x = " << (i + 1) * m - j << " (" << pow_mod(a, (i + 1) * m - j, p) << ")\n";
                break;
            }
        }
    }
    delete[] Ay;
    delete[] A;
}

long long int shamir(long long int m)
{
    long long int p = 0, cA = 0, cB = 0, dA = 0, dB = 0, x[4];
    while ((p == 0) || (p <= m) || !simple(p))
        p = rand() % (long long int)pow(10, 9);

    while ((cA == 0) || (evk(cA, p - 1, dA, x[0]) != 1))
        cA = rand() % (long long int)pow(10, 9);

    while ((cB == 0) || (evk(cB, p - 1, dB, x[0]) != 1))
        cB = rand() % (long long int)pow(10, 9);

    while (dA < 0)
        dA += (p - 1);
    while (dB < 0)
        dB += (p - 1);
    x[0] = pow_mod(m, cA, p);
    x[1] = pow_mod(x[0], cB, p);
    x[2] = pow_mod(x[1], dA, p);
    x[3] = pow_mod(x[2], dB, p);
    return x[3];
}

long long int vernam(long long int m)
{
    long long int i, n, n2, x = m, k, e;
    for (n = 0, x = m; x > 0; n++, x /= 2);
    do
    {
        k = dh();
        for (n2 = 0, x = k; x > 0; n2++, x /= 2);
    } while (n != n2);
    e = m ^ k;
    return e ^ k;
}

long long int elgam(long long int m)
{
    long long int p = 0, q = 0, g = 0, c[2], d[2], k, r, e, m2;
    while (!simple(q) || (q == 0) || !simple(p) || (p <= m))
    {
        q = rand() % (long long int)pow(10, 9);
        p = 2 * q + 1;
    }
    do
    {
        g = 2 + rand() % (p - 2);
    } while (pow_mod(g, q, p) == 1);
    c[0] = 2 + rand() % (p - 1);
    d[0] = pow_mod(g, c[0], p);
    c[1] = 2 + rand() % (p - 1);
    d[1] = pow_mod(g, c[1], p);

    k = 1 + rand() % (p - 1);
    r = pow_mod(g, k, p);
    e = ((m % p) * pow_mod(d[1], k, p)) % p;

    m2 = ((e % p) * pow_mod(r, p - 1 - c[1], p)) % p;
    return m2;
}

void gen(long long int& c, long long int& d, long long int& n)
{
    long long int p = 0, q = 0, f, x;
    d = 0;
    while ((p == 0) || !simple(p))
        p = rand() % (long long int)pow(10, 6);
    while ((q == 0) || !simple(q))
        q = rand() % (long long int)pow(10, 6);
    n = p * q;
    f = (p - 1) * (q - 1);
    while ((d == 0) || (d >= f) || (evk(f, d, x, c) != 1))
        d = rand() % (long long int)pow(10, 6);
    while (c < 0)
        c += f;
    cout << " n = " << n << ", p = " << p << ", q = " << q << ", f = " << f << endl;
    cout << " c = " << c << ", d = " << d << " | " << ((c % f) * (d % f)) % f << endl;
}

long long int rsa(long long int m)
{
    long long int i, c[2], d[2], n[2], e, f, u, m2;
    for (i = 0; i < 2; i++)
    {
        gen(c[i], d[i], n[i]);
        if (m >= n[i])
            i--;
    }

    cout << " ca = " << c[0] << ", da = " << d[0] << endl;
    cout << " cb = " << c[1] << ", db = " << d[1] << endl;
    e = pow_mod(m, c[0], n[0]);
    f = pow_mod(e, d[1], n[1]);
    u = pow_mod(f, c[1], n[1]);
    m2 = pow_mod(u, d[0], n[0]);
    cout << " e = " << e << ", f = " << f << ", u = " << u << endl;
    cout << " m = " << m << ", m2 = " << m2 << endl << endl;
    return m2;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(0));
    int m = 25;
    rsa(m);
    /*ifstream f1("..\\..\\file1.txt");
    ofstream f2("..\\..\\file2.txt"), f3("..\\..\\file3.txt"), f4("..\\..\\file4.txt"), f5("..\\..\\file5.txt");
    char ch;
    while (f1.get(ch))
    {
        f2 << (char)shamir((long long int)ch);
        f3 << (char)vernam((long long int)ch);
        f4 << (char)elgam((long long int)ch);
        f5 << (char)rsa((long long int)ch);
    }
    f1.close();
    f2.close();
    f3.close();
    f4.close();
    f5.close();*/
    return 0;
}