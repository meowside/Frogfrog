using System;

namespace MVC_WebService.Models
{
    public class PredmetLocalModel
    {
        public Guid Id { get; set; }
        public bool? Povinny { get; set; }
        public string Predmet { get; set; }
    }
}