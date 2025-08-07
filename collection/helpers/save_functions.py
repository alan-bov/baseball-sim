import json
import sqlite3
from pathlib import Path

BASE_DIR = Path(__file__).resolve().parents[2]
DATA_DIR = BASE_DIR / "data"
DATA_DIR.mkdir(parents=True, exist_ok=True)

def save_pitcher(pitcher):
    db_path = DATA_DIR / 'pitchers.db'
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS pitchers (
            first_name TEXT,
            last_name TEXT,
            year TEXT,
            player_id INTEGER,
            pitch_mix TEXT,
            count_usage TEXT,
            PRIMARY KEY (first_name, last_name, year)
        )
    ''')

    pitch_mix_json = json.dumps(pitcher.pitch_mix)
    count_usage_json = json.dumps(pitcher.count_usage)

    cursor.execute('''
        INSERT OR REPLACE INTO pitchers (first_name, last_name, year, player_id, pitch_mix, count_usage)
        VALUES (?, ?, ?, ?, ?, ?)
    ''', (pitcher.first_name, pitcher.last_name, pitcher.year, pitcher.player_id, pitch_mix_json, count_usage_json))

    conn.commit()
    conn.close()

def save_batter(batter):
    db_path = DATA_DIR / 'batters.db'
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS batters (
            first_name TEXT,
            last_name TEXT,
            year TEXT,
            player_id INTEGER,
            PRIMARY KEY (first_name, last_name, year)
        )
    ''')

    cursor.execute('''
        INSERT OR REPLACE INTO batters (first_name, last_name, year, player_id)
        VALUES (?, ?, ?, ?)
    ''', (batter.first_name, batter.last_name, batter.year, batter.player_id))

    conn.commit()
    conn.close()