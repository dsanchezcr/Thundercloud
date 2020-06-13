using System;
using System.Text;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Extensions.Http;
using Microsoft.Azure.Devices;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.Logging;

namespace ThunderCloud
{
    public static class SendThunder
    {
        static readonly string connectionString = Environment.GetEnvironmentVariable("IoTHubConnectionString");
        static readonly RegistryManager registryManager = RegistryManager.CreateFromConnectionString(connectionString);
        private static readonly ServiceClient serviceClient = ServiceClient.CreateFromConnectionString(connectionString);

        [FunctionName("SendThunder")]
        public static async Task<IActionResult> Run(
            [HttpTrigger(AuthorizationLevel.Anonymous, "post", Route = null)] HttpRequest req,
            ILogger log)
        {
            log.LogInformation("New thunder just came through a HTTP Request.");

            var commandMessage = new Message(Encoding.ASCII.GetBytes("Thunder"));
            var device = await registryManager.GetDeviceAsync("MXChip");
            await serviceClient.SendAsync(device.Id, commandMessage);

            return new OkResult();
        }
    }
}
