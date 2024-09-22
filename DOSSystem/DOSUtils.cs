using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DOSSystem
{
    public static class DOSUtils
    {
        public static uint UnixTimeStamp()
        {
            var timeSpan = (DateTime.UtcNow - new DateTime(1970, 1, 1, 0, 0, 0));
            return (uint)timeSpan.TotalSeconds;

        }

        public static DateTime UnixTimeStampToDateTime(uint TimeStamp)
        {
            DateTime Time = new DateTime(1970, 1, 1, 0, 0, 0);
            Time = Time.Add(TimeSpan.FromTicks(TimeStamp * TimeSpan.TicksPerSecond));
            return TimeZone.CurrentTimeZone.ToLocalTime(Time);
        }

        public static double Saturate(double Value, double Min, double Max)
        {
            if (Value < Min)
                return Min;
            if (Value > Max)
                return Max;
            return Value;
        }
        public static float Saturate(float Value, float Min, float Max)
        {
            if (Value < Min)
                return Min;
            if (Value > Max)
                return Max;
            return Value;
        }
        public static float Saturate(float Value, float Min)
        {
            if (Value < Min)
                return Min;
            return Value;
        }

        static System.Random Rnd = new System.Random(unchecked((int)DateTime.Now.Ticks));

        public static float GetRandom()
        {
            return (float)Rnd.NextDouble();
        }
        public static float GetRandom(float Min, float Max)
        {
            return (float)(Min + (Max - Min) * Rnd.NextDouble());
        }

        public static int GetRandom(int Min, int Max)
        {
            return Min + (int)Math.Ceiling((Max - Min) * Rnd.NextDouble());
        }

        public static uint GetRandom(uint Min, uint Max)
        {
            return Min + (uint)Math.Ceiling((Max - Min) * Rnd.NextDouble());
        }

        public static double GetRandomGauss(double Min, double Max)
        {
            double r = Rnd.NextDouble();
            double sigma = (Max - Min) / 6.0f;
            double mu = (Max + Min) / 2.0f;
            return Saturate((mu + (r > 0.5 ? -1.0 : 1.0) * sigma * Math.Pow(-Math.Log(0.99999f * r + 0.0000001f), 0.5)), Min, Max);
        }
        public static int GetRandomGauss(int Min, int Max)
        {
            return (int)GetRandomGauss((double)Min, (double)Max);
        }

        public static uint GetRandomGauss(uint Min, uint Max)
        {
            return (uint)GetRandomGauss((double)Min, (double)Max);
        }
    }
}
