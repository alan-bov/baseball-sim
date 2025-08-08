import json
import sqlite3
from pathlib import Path

def test_read_pitchers_db(name, year_input):
    # Set path relative to this script: ../../data/pitchers.db
    BASE_DIR = Path(__file__).resolve().parents[2]  # Adjust depending on where this file lives
    db_path = BASE_DIR / "data" / "pitchers.db"

    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()

    cursor.execute("SELECT first_name, last_name, year, player_id, pitch_mix, usage_vs_righty, usage_vs_lefty FROM pitchers")
    rows = cursor.fetchall()

    if not rows:
        print("No data found in pitchers.db")
        return

    for row in rows:

        first_name, last_name, year, player_id, pitch_mix_json, usage_vs_righty_json, usage_vs_lefty_json = row
        print(f"{first_name} {last_name} ({year}) - Player ID: {player_id}")

        if f"{first_name} {last_name}" != name or year != year_input:
            continue
        
        pitch_mix = json.loads(pitch_mix_json)
        usage_vs_righty = json.loads(usage_vs_righty_json)
        usage_vs_lefty = json.loads(usage_vs_lefty_json)

        print(f"  Pitch Mix: {pitch_mix}")
        print(f"  Count Usage vs. Righty: {usage_vs_righty}")
        print(f"  Count Usage vs. Lefty: {usage_vs_lefty}")
        print("-" * 80)

    conn.close()

def test_read_batters_db(name, year_input):
    BASE_DIR = Path(__file__).resolve().parents[2]  # Adjust as needed
    db_path = BASE_DIR / "data" / "batters.db"

    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()

    cursor.execute("""
        SELECT first_name, last_name, year, player_id, hand, pitch_probability_vs_righty, pitch_probability_vs_lefty 
        FROM batters
    """)
    rows = cursor.fetchall()

    if not rows:
        print("No data found in batters.db")
        return

    for row in rows:
        first_name, last_name, year, player_id, hand, righty_json, lefty_json = row
        full_name = f"{first_name} {last_name}"

        if full_name != name or year != year_input:
            continue

        print(f"{full_name} ({year}) - Player ID: {player_id} - Hand: {hand}")

        pitch_probability_vs_righty = json.loads(righty_json)
        pitch_probability_vs_lefty = json.loads(lefty_json)

        print(f"  Pitch Probability vs Righty:")
        for balls, counts in pitch_probability_vs_righty.items():
            for strikes, pitch_probs in counts.items():
                print(f"    Count {balls}-{strikes}: {pitch_probs}")

        print(f"  Pitch Probability vs Lefty:")
        for balls, counts in pitch_probability_vs_lefty.items():
            for strikes, pitch_probs in counts.items():
                print(f"    Count {balls}-{strikes}: {pitch_probs}")

        print("-" * 80)

    conn.close()

if __name__ == "__main__":
    player = "Riley Greene"
    year = "2024"
    #test_read_pitchers_db(player, year)
    test_read_batters_db(player, year)
