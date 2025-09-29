import ipaddress
import csv

def sort_by_ip_and_mask(input_filename, output_filename):
    with open(input_filename, newline='', encoding='windows-1250') as file:
        reader = csv.reader(file, delimiter=';')
        header = next(reader)
        rows = list(reader)

    def sort_key(row):
        try:
            ip = ipaddress.IPv4Address(row[2])
        except Exception:
            ip = ipaddress.IPv4Address("255.255.255.255")  # Put broken rows last

        try:
            mask = int(row[3])
        except Exception:
            mask = 255  # Treat broken masks as max value

        return (ip, mask)

    sorted_rows = sorted(rows, key=sort_key)

    with open(output_filename, 'w', newline='', encoding='windows-1250') as file:
        writer = csv.writer(file, delimiter=';')
        writer.writerow(header)
        writer.writerows(sorted_rows)

    print(f"✅ Included all rows, written sorted data to '{output_filename}'")

# Example usage
sort_by_ip_and_mask(
    'C:/Users/Peter/Desktop/AUS2Pokus/Semestrákla/AUS-VS2022/SemPraca/RTUprava1.csv',
    'C:/Users/Peter/Desktop/AUS2Pokus/Semestrákla/AUS-VS2022/SemPraca/RTUprava2.csv'
)
