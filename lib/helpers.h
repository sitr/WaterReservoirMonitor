long microsecondsToCentimeters(long microseconds)
{
    // The speed of sound is 340 m/s or 29 microseconds per centimetre.
    // The ping travels out and back, so to find the distance of the
    // object we take half of the distance travelled.
    return (microseconds / 2) / 29.2;
}