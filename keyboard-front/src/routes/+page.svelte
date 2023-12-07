<script lang="ts">
 import { Button, FloatingLabelInput, Helper } from 'flowbite-svelte';
 type Suggestions = { suggestions: string[] };
 let userInput: any = '';
 async function getSuggestions(text: string): Promise<Suggestions> {
     return fetch("/suggestion", {
         method: "post",
         headers: {
             "Accept": "application/json",
             "Content-Type": "application/json"
         },
         body: JSON.stringify({
             "text": text
         })
     })
         .then((resp) => {
             let result = resp.json();
             console.log(result);
             return result;
         })
 }
 let suggestions: Promise<Suggestions>;
 function getSlug(amount: number): string[] {
     let slug: string[] = [];
     for (let i = 0; i < amount; i++) {
         slug.push('Working...');
     }
     return slug
 }
 function appendInput(word: string) {
     let words = userInput.split(' ');
     words[words.length - 1] = word;
     userInput = words.join(' ') + ' ';
 }
 const slug = getSlug(10);
</script>
<div class="container flex flex-col p-12 space-y-1">
    <FloatingLabelInput style="outlined" type="text" bind:value={userInput}></FloatingLabelInput>
    <div class="flex-row space-x-1">
        {#await getSuggestions(userInput)}
            <Button color="alternative">Working...</Button>
        {:then val}
            {#each val.suggestions as word}
                <Button on:click={appendInput(word)} color="alternative">{word}</Button>
            {/each}
        {:catch error}
            <p style="color: red;">{error.message}</p>
        {/await}
    </div>
</div>
