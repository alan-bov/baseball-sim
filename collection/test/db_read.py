import json
import sqlite3
from pathlib import Path

def test_read_pitchers_db():
    # Set path relative to this script: ../../data/pitchers.db
    BASE_DIR = Path(__file__).resolve().parents[2]  # Adjust depending on where this file lives
    db_path = BASE_DIR / "data" / "pitchers.db"

    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()

    cursor.execute("SELECT first_name, last_name, year, player_id, pitch_mix, count_usage FROM pitchers")
    rows = cursor.fetchall()

    if not rows:
        print("No data found in pitchers.db")
        return

    for row in rows:
        first_name, last_name, year, player_id, pitch_mix_json, count_usage_json = row
        print(f"{first_name} {last_name} ({year}) - Player ID: {player_id}")
        
        pitch_mix = json.loads(pitch_mix_json)
        count_usage = json.loads(count_usage_json)

        print(f"  Pitch Mix: {pitch_mix}")
        print(f"  Count Usage: {count_usage}")
        print("-" * 40)

    conn.close()

if __name__ == "__main__":
    test_read_pitchers_db()
