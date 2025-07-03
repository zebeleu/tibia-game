package main

import (
	"crypto/rsa"
	"crypto/x509"
	"encoding/pem"
	"math/big"
	"os"
)

const (
	P = "1201758001370723323398753778257470257713354828752713123415294815" +
		"0506251412291888866940292054989907714155267326586216043845592229" +
		"084368540020196135619327879"

	Q = "1189892136861686835188050824611210139447876026576932541274639840" +
		"5473436969889506919017477758618276066588858607419440134394668095" +
		"105156501566867770737187273"

	E = "65537"
)

func main() {
	// NOTE(fusion): Generate key from known P, Q, and E. There isn't a helper
	// function from the standard library so we need to build the private key
	// ourselves.
	p, ok := new(big.Int).SetString(P, 10)
	if !ok || !p.ProbablyPrime(4) {
		panic("invalid P")
	}

	q, ok := new(big.Int).SetString(Q, 10)
	if !ok || !q.ProbablyPrime(4) {
		panic("invalid Q")
	}

	e, ok := new(big.Int).SetString(E, 10)
	if !ok || !e.IsInt64() || !e.ProbablyPrime(4) {
		panic("invalid E")
	}

	pMinus1 := new(big.Int).Sub(p, big.NewInt(1))
	qMinus1 := new(big.Int).Sub(q, big.NewInt(1))
	gcd := new(big.Int).GCD(nil, nil, pMinus1, qMinus1)
	phi := new(big.Int).Mul(pMinus1, qMinus1)
	lambda := new(big.Int).Div(phi, gcd)
	d := new(big.Int).ModInverse(e, lambda)
	n := new(big.Int).Mul(p, q)

	privateKey := rsa.PrivateKey{
		PublicKey: rsa.PublicKey{
			N: n,
			E: int(e.Int64()),
		},
		D:      d,
		Primes: []*big.Int{p, q},
	}

	// NOTE(fusion): `Validate()` will only perform minor sanity checks. To actually
	// check that the output key is valid use `openssl rsa -in KEY.PEM -check`.
	if err := privateKey.Validate(); err != nil {
		panic("invalid private key: " + err.Error())
	}

	// NOTE(fusion): Dump private key into stdout.
	block := pem.Block{
		Type:  "RSA PRIVATE KEY",
		Bytes: x509.MarshalPKCS1PrivateKey(&privateKey),
	}
	pem.Encode(os.Stdout, &block)
}
