import os

def zbierz_dane_cpp_h_md(sciezka_glowna, plik_wyjsciowy):
    """
    Zbiera dane z plików .cpp i .h w folderze głównym i podfolderach,
    a następnie zapisuje je do jednego pliku .md.
    Ignoruje pliki, których nazwa zawiera "(Ignore)".

    Args:
        sciezka_glowna (str): Ścieżka do folderu głównego.
        plik_wyjsciowy (str): Nazwa pliku wyjściowego.
    """

    with open(plik_wyjsciowy, 'w', encoding='utf-8') as plik_wynikowy:
        for korzen, _, pliki in os.walk(sciezka_glowna):
            for plik in pliki:
                if "(Ignore)" not in plik:
                    if plik.endswith('.h') or plik.endswith('.cpp'):
                        sciezka_pliku = os.path.join(korzen, plik)
                        try:
                            with open(sciezka_pliku, 'r', encoding='utf-8') as plik_cpp_h:
                                plik_wynikowy.write(f'## {plik}\n')
                                plik_wynikowy.write(plik_cpp_h.read())
                                plik_wynikowy.write('\n****\n')
                        except UnicodeDecodeError:
                            print(f"Błąd dekodowania UTF-8 w pliku {sciezka_pliku}. Próbuję z latin-1.")
                            try:
                                with open(sciezka_pliku, 'r', encoding='latin-1') as plik_cpp_h:
                                    plik_wynikowy.write(f'## {plik}\n')
                                    plik_wynikowy.write(f'## {plik}\n')
                                    plik_wynikowy.write(plik_cpp_h.read())
                                    plik_wynikowy.write('\n****\n')
                            except Exception as e:
                                print(f"Nie udało się odczytać pliku {sciezka_pliku}: {e}")
                                continue

# Użycie skryptu
sciezka_glowna = '.'  # Bieżący folder
plik_wyjsciowy = 'KOD.md'
zbierz_dane_cpp_h_md(sciezka_glowna, plik_wyjsciowy)

print(f'Dane z plików .cpp i .h zostały zebrane i zapisane do pliku: {plik_wyjsciowy}')