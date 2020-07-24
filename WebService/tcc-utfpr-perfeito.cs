using System;
using System.IO;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Extensions.Http;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;

namespace app.perfeito
{
    public static class tcc_utfpr_perfeito
    {
        [FunctionName("verify_access")]
        public static async Task<IActionResult> Run(
            [HttpTrigger(AuthorizationLevel.Anonymous, "get", Route = null)] HttpRequest req,
            ILogger log)
        {
            log.LogInformation("Function processed a request.");

            string codeId = req.Query["code_id"];

            string requestBody = await new StreamReader(req.Body).ReadToEndAsync();
            dynamic data = JsonConvert.DeserializeObject(requestBody);
            codeId = codeId ?? data?.name;

            if (!string.IsNullOrEmpty(codeId))
            {
                if(codeId.Equals("8467287462376492"))
                {
                    var result1 = new ObjResult("true", "access granted");
                    return new OkObjectResult(result1);
                }
            }

            var result = new ObjResult("false", "access denied");
            return new BadRequestObjectResult(result);
        }
    }

    public class ObjResult {

        public ObjResult(String status, String message) 
        { 
            this.status = status; 
            this.message = message; 
        } 

        public string status;
        public string message;
    }
}