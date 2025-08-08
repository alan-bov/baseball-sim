from collections import defaultdict
from pybaseball import playerid_lookup, statcast_pitcher, statcast_batter

class Pitcher:
    def __init__(self, first_name, last_name, year):
        # Name and year
        self.first_name = first_name
        self.last_name = last_name
        self.year = year

        self.full_name = self.first_name + ' ' + self.last_name

        # Data
        self.pitch_mix = []
        self.usage_vs_righty = defaultdict(lambda: defaultdict(dict))
        self.usage_vs_lefty = defaultdict(lambda: defaultdict(dict))
        self.ops_against_pp_righty = defaultdict(lambda: defaultdict(dict))
        self.ops_against_pp_lefty = defaultdict(lambda: defaultdict(dict))
        self.ops_per_count = defaultdict(lambda: defaultdict(dict))

        # Miscellaneous
        self.player_id = None
        self.hand = ''

    def define_id(self, player_id):
        self.player_id = player_id
    
    def add_pitch(self, pitch_type):
        self.pitch_mix.append(pitch_type)
    
    def show_pitch_mix(self):
        print(f"Pitch mix for {self.full_name} [{self.player_id}]: {self.pitch_mix}")

    def show_total_count_usage(self, hand):
        if hand == 'Right':
            usage = self.usage_vs_righty
        elif hand == 'Left':
            usage = self.usage_vs_lefty
        else:
            print("Invalid hand specified. Use 'right' or 'left'.")
            return

        print(f"{self.full_name} [{self.player_id}] Pitch Mix By Count Against {hand}y:")
        for balls in range(4):
            for strikes in range(3):
                print(f"   {balls}-{strikes} count usage: {usage[balls][strikes]}")
    
    def show_count_usage(self, balls, strikes, hand):
        if hand == 'Right':
            usage = self.usage_vs_righty
        elif hand == 'Left':
            usage = self.usage_vs_lefty
        else:
            print("Invalid hand specified. Use 'Right' or 'Left'.")
            return
        print(f"{self.full_name} [{self.player_id}] Pitch Mix For {balls}-{strikes} Count Against {hand}y:")
        print(f"   {balls}-{strikes} count usage: {usage[balls][strikes]}")

class Batter:
    def __init__(self, first_name, last_name, year):
        # Name and year
        self.first_name = first_name
        self.last_name = last_name
        self.year = year

        self.full_name = self.first_name + ' ' + self.last_name

        # Data
        self.pitch_probability_vs_righty = defaultdict(lambda: defaultdict(dict))
        self.pitch_probability_vs_lefty = defaultdict(lambda: defaultdict(dict))

        # Miscellaneous
        self.player_id = None
        self.hand = ''

    def define_id(self, player_id):
        self.player_id = player_id

    def show_total_pitch_probability(self, hand):
        if hand == 'Right':
            pitch_probability = self.pitch_probability_vs_righty
        elif hand == 'Left':
            pitch_probability = self.pitch_probability_vs_lefty
        else:
            print("Invalid hand specified. Use 'right' or 'left'.")
            return

        print(f"{self.full_name} [{self.player_id}] Probability Of Pitch Seen By Count Against {hand}y:")
        for balls in range(4):
            for strikes in range(3):
                print(f"   {balls}-{strikes} count pitch probability: {pitch_probability[balls][strikes]}")
    
    def show_count_usage(self, balls, strikes, hand):
        if hand == 'Right':
            pitch_probability = self.pitch_probability_vs_righty
        elif hand == 'Left':
            pitch_probability = self.pitch_probability_vs_lefty
        else:
            print("Invalid hand specified. Use 'right' or 'left'.")
            return
        
        print(f"{self.full_name} [{self.player_id}] Probability Of Pitch Seen For {balls}-{strikes} Count Against {hand}y:")
        print(f"   {balls}-{strikes} count pitch probability: {pitch_probability[balls][strikes]}")


def pitcher_lookup(pitcher):
    try:
        # Get player ID from pybaseball
        player_info = playerid_lookup(pitcher.last_name, pitcher.first_name)
        pitcher.define_id(int(player_info['key_mlbam'].iloc[0]))

        # Pull data from specified year and get handedness
        start_date = f"{pitcher.year}-03-01"
        end_date = f"{pitcher.year}-09-30"
        data = statcast_pitcher(start_dt=start_date, end_dt=end_date, player_id=pitcher.player_id)

        hand_map = {"R": "Right", "L": "Left", "S": "Switch"}
        if not data.empty:
            pitcher_hand_code = data['p_throws'].iloc[0]  # usually 'R' or 'L'
            pitcher.hand = hand_map.get(pitcher_hand_code, "Unknown")
        else:
            pitcher.hand = "Unknown"

        # Filter by batter handedness
        data_against_righty = data[data['stand'] == 'R']
        data_against_lefty = data[data['stand'] == 'L']

        # Build pitch mix
        pitch_types = data['pitch_type'].value_counts()
        for pitch_type, count in pitch_types.items():
            pitcher.add_pitch(pitch_type)
        
        # Filter usage by count, top for righty, bottom for lefty
        for balls in range(4):
            for strikes in range(3):
                subset = data_against_righty[(data_against_righty['balls'] == balls) & (data_against_righty['strikes'] == strikes)]
                if subset.empty:
                    continue

                pitch_counts = subset['pitch_type'].value_counts()
                total = pitch_counts.sum()

                if total > 0:
                    pitch_percents = (pitch_counts / total).round(3).to_dict()
                    pitcher.usage_vs_righty[balls][strikes] = pitch_percents

        for balls in range(4):
            for strikes in range(3):
                subset = data_against_lefty[(data_against_lefty['balls'] == balls) & (data_against_lefty['strikes'] == strikes)]
                if subset.empty:
                    continue

                pitch_counts = subset['pitch_type'].value_counts()
                total = pitch_counts.sum()

                if total > 0:
                    pitch_percents = (pitch_counts / total).round(3).to_dict()
                    pitcher.usage_vs_lefty[balls][strikes] = pitch_percents

        # Parse other important data

        return pitcher
    except Exception as e:
        print(f"Exception thrown while gathering pitcher data: {e}")
        return None

def batter_lookup(batter):
    try:
        # Get player ID from pybaseball
        player_info = playerid_lookup(batter.last_name, batter.first_name)
        batter.define_id(int(player_info['key_mlbam'].iloc[0]))

        # Pull data from specified year and get handedness
        start_date = f"{batter.year}-03-01"
        end_date = f"{batter.year}-09-30"
        data = statcast_batter(start_dt=start_date, end_dt=end_date, player_id=batter.player_id)

        hand_map = {"R": "Right", "L": "Left", "S": "Switch"}
        if not data.empty:
            batter_hand_code = data['stand'].iloc[0]  # 'R', 'L', or 'S'
            batter.hand = hand_map.get(batter_hand_code, "Unknown")
        else:
            batter.hand = "Unknown"

        # Filter by pitcher handedness
        data_against_righty = data[data['p_throws'] == 'R']
        data_against_lefty = data[data['p_throws'] == 'L']

        # Filter usage by count, top for righty, bottom for lefty
        for balls in range(4):
            for strikes in range(3):
                subset = data_against_righty[(data_against_righty['balls'] == balls) & (data_against_righty['strikes'] == strikes)]
                if subset.empty:
                    continue

                pitch_counts = subset['pitch_type'].value_counts()
                total = pitch_counts.sum()

                if total > 0:
                    batter.pitch_probability_vs_righty[balls][strikes] = (pitch_counts / total).round(3).to_dict()

        for balls in range(4):
            for strikes in range(3):
                subset = data_against_lefty[(data_against_lefty['balls'] == balls) & (data_against_lefty['strikes'] == strikes)]
                if subset.empty:
                    continue

                pitch_counts = subset['pitch_type'].value_counts()
                total = pitch_counts.sum()

                if total > 0:
                    batter.pitch_probability_vs_lefty[balls][strikes] = (pitch_counts / total).round(3).to_dict()

        # Parse other important data

        return batter
    except Exception as e:
        print(f"Exception thrown while gathering batter data: {e}")
        return None
