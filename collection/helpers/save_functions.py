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
            hand TEXT,
            pitch_mix TEXT,
            usage_vs_righty TEXT,
            usage_vs_lefty TEXT,
            PRIMARY KEY (first_name, last_name, year)
        )
    ''')

    pitch_mix_json = json.dumps(pitcher.pitch_mix)
    usage_vs_righty_json = json.dumps(pitcher.usage_vs_righty)
    usage_vs_lefty_json = json.dumps(pitcher.usage_vs_lefty)

    cursor.execute('''
        INSERT OR REPLACE INTO pitchers (first_name, last_name, year, player_id, hand, pitch_mix, usage_vs_righty, usage_vs_lefty)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?)
    ''', (
        pitcher.first_name,
        pitcher.last_name,
        pitcher.year,
        pitcher.player_id,
        pitcher.hand,
        pitch_mix_json,
        usage_vs_righty_json,
        usage_vs_lefty_json
        ))

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
            hand TEXT,
            pitch_probability_vs_righty TEXT,
            pitch_probability_vs_lefty TEXT,
            PRIMARY KEY (first_name, last_name, year)
        )
    ''')

    pitch_probability_vs_righty_json = json.dumps(batter.pitch_probability_vs_righty)
    pitch_probability_vs_lefty_json = json.dumps(batter.pitch_probability_vs_lefty)

    cursor.execute('''
        INSERT OR REPLACE INTO batters 
        (first_name, last_name, year, player_id, hand, pitch_probability_vs_righty, pitch_probability_vs_lefty)
        VALUES (?, ?, ?, ?, ?, ?, ?)
    ''', (
        batter.first_name,
        batter.last_name,
        batter.year,
        batter.player_id,
        batter.hand,
        pitch_probability_vs_righty_json,
        pitch_probability_vs_lefty_json
    ))

    conn.commit()
    conn.close()