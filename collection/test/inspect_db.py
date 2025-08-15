import sqlite3

db_path = r"C:\\Users\\abovee\\Documents\\Python Projects\\BB Model\\data\\pitchers.db"

conn = sqlite3.connect(db_path)
cursor = conn.cursor()

# List all tables
cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
print("Tables:", cursor.fetchall())

# Inspect the pitchers table
cursor.execute("PRAGMA table_info(pitchers);")
for row in cursor.fetchall():
    print(row)

conn.close()