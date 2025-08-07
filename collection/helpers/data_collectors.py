from collections import defaultdict
from pybaseball import playerid_lookup, statcast_pitcher

class Pitcher:
    def __init__(self, first_name, last_name, year):
        self.first_name = first_name
        self.last_name = last_name
        self.year = year

        self.full_name = self.first_name + ' ' + self.last_name

        self.pitch_mix = []
        self.count_usage = defaultdict(lambda: defaultdict(dict))

        self.player_id = None
        self.team = ''
        self.pos = ''
        self.hand = ''

    def define_id(self, player_id):
        self.player_id = player_id
    
    def add_pitch(self, pitch_type):
        self.pitch_mix.append(pitch_type)
    
    def show_pitch_mix(self):
        print(f"Pitch mix for {self.full_name} [{self.player_id}]: {self.pitch_mix}")

    def show_total_count_usage(self):
        print(f"{self.full_name} [{self.player_id}] Pitch Mix By Count:")
        for balls in range(4):
            for strikes in range(3):
                print(f"   {balls}-{strikes} count usage: {self.count_usage[balls][strikes]}")
    
    def show_count_usage(self, balls, strikes):
        print(f"{self.full_name} [{self.player_id}] Pitch Mix For {balls}-{strikes} Count:")
        print(f"   {balls}-{strikes} count usage: {self.count_usage[balls][strikes]}")

class Batter:
    def __init__(self, first_name, last_name, year):
        self.first_name = first_name
        self.last_name = last_name
        self.year = year

        self.full_name = self.first_name + ' ' + self.last_name

        self.player_id = None
        self.team = ''
        self.pos = ''
        self.hand = ''

def pitcher_lookup(pitcher):
    try:
        # Get player ID from pybaseball
        player_info = playerid_lookup(pitcher.last_name, pitcher.first_name)
        pitcher.define_id(int(player_info['key_mlbam'].iloc[0]))

        # Pull data from specified year
        start_date = f"{pitcher.year}-03-01"
        end_date = f"{pitcher.year}-09-30"
        data = statcast_pitcher(start_dt=start_date, end_dt=end_date, player_id=pitcher.player_id)

        # Build pitch mix
        pitch_types = data['pitch_type'].value_counts()
        for pitch_type, count in pitch_types.items():
            pitcher.add_pitch(pitch_type)
        
        # Filter usage by count
        for balls in range(4):
            for strikes in range(3):
                subset = data[(data['balls'] == balls) & (data['strikes'] == strikes)]
                if subset.empty:
                    continue

                pitch_counts = subset['pitch_type'].value_counts()
                total = pitch_counts.sum()

                if total > 0:
                    pitch_percents = (pitch_counts / total).round(4).to_dict()
                    pitcher.count_usage[balls][strikes] = pitch_percents

        # Parse other important data

        return pitcher
    except Exception as e:
        print(f"Exception thrown while gathering pitcher data: {e}")
        return pitcher

def batter_lookup(batter):
    return batter