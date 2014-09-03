import subprocess, sys
def join(*listas):
	tamanho = max(map(len, listas))
	c = 0
	while c < tamanho:
		yield [None if c >= len(lista) else lista[c] for lista in listas]
		c += 1 
content = subprocess.Popen([sys.argv[1]], stdout=subprocess.PIPE).communicate()[0].strip()
lines = content.split("\n")
A = map(lambda n: int(n.strip()), lines[0].split("=", 1)[1].strip().split(","))
B = map(lambda n: int(n.strip()), lines[1].split("=", 1)[1].strip().split(","))
produto_escalar_original = int(lines[-1].split("=",1)[1].strip())
produto_escalar_resultado = sum(map(lambda item: item[0]*item[1], list(join(A, B))))
print "-"*100
print content
print "-"*100
if produto_escalar_original == produto_escalar_resultado:
	print "Resultado igual: O produto escalar total esta correto (%d (original) == %d (calculado))"%(produto_escalar_original, produto_escalar_resultado)
else:
	print "Resultado diferente: O produto escalar total esta incorreto (%d (original) != %d (calculado))"%(produto_escalar_original, produto_escalar_resultado)
